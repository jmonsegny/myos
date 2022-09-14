#include <drivers/vga.h>
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;

VideoGraphicsArray::
VideoGraphicsArray()
:_miscPort(0x3C2),
_crtcIndexPort(0x3D4),
_crtcDataPort(0x3D5),
_sequencerIndexPort(0x3C4),
_sequencerDataPort(0x3C5),
_graphicsControllerIndexPort(0x3CE),
_graphicsControllerDataPort(0x3CF),
_attributeControllerIndexPort(0x3C0),
_attributeControllerReadPort(0x3C1),
_attributeControllerWritePort(0x3C0),
_attributeControllerResetPort(0x3DA)
{
}

VideoGraphicsArray::
~VideoGraphicsArray()
{
}

void VideoGraphicsArray::
writeRegisters( uint8_t* registers )
{
	_miscPort.write( registers[0] );
	for( uint8_t i = 1; i < 6; i++ ) {
		_sequencerIndexPort.write( i-1 );
		_sequencerDataPort.write( registers[i] );
	}

	_crtcIndexPort.write(0x03);
	_crtcDataPort.write( _crtcDataPort.read() | 0x80 );
	_crtcIndexPort.write(0x11);
    _crtcDataPort.write( _crtcDataPort.read() & ~0x80 );
	registers[0x03] = registers[0x03] | 0x80;
	registers[0x11] = registers[0x11] & ~0x80;

	for( uint8_t i = 6; i < 31; i++ ) {
		_crtcIndexPort.write( i-6 );
		_crtcDataPort.write( registers[i] );
	}

	for( uint8_t i = 31; i < 40; i++ ) {
		_graphicsControllerIndexPort.write( i-31 );
		_graphicsControllerDataPort.write( registers[i] );
	}

	for( uint8_t i = 40; i < 61; i++ ) {
		_attributeControllerResetPort.read();
		_attributeControllerIndexPort.write( i-40 );
		_attributeControllerWritePort.write( registers[i] );		
	}

	_attributeControllerResetPort.read();
	_attributeControllerIndexPort.write(0x20);
}

uint8_t* VideoGraphicsArray::
getFrameBufferSegment()
{
	_graphicsControllerIndexPort.write(0x06);
	uint8_t segmentNumber = ((_graphicsControllerDataPort.read() >> 2) & 0x03 );
	switch( segmentNumber ){
		default:
		case 0x00: return (uint8_t*)0x00000; break;
		case 0x01: return (uint8_t*)0xA0000; break;
		case 0x02: return (uint8_t*)0xB0000; break;
		case 0x03: return (uint8_t*)0xB8000; break;
	}
}

void VideoGraphicsArray::
putPixel( uint32_t x, uint32_t y, uint8_t colorIndex )
{
	uint8_t* pixelAddress = getFrameBufferSegment() + 320*y + x;
	*pixelAddress = colorIndex;
}

uint8_t VideoGraphicsArray::
getColorIndex( uint8_t r, uint8_t g, uint8_t b )
{
	if( r == 0x00 && g == 0x00 && b == 0xA8 )
		return 0x01;
	return 0x00; // ?
}

bool VideoGraphicsArray::
supportsMode( uint32_t width, uint32_t height, uint32_t colordepth )
{
	return (width == 320 && height == 200 && colordepth == 8);
}

bool VideoGraphicsArray::
setMode( uint32_t width, uint32_t height, uint32_t colordepth )
{
	if( !supportsMode( width, height, colordepth) )
		return false;

	uint8_t g_320x200x256[] =
	{
		/* MISC */
		0x63,
		/* SEQ */
		0x03, 0x01, 0x0F, 0x00, 0x0E,
		/* CRTC */
		0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
		0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
		0xFF,
		/* GC */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
		0xFF,
		/* AC */
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		0x41, 0x00, 0x0F, 0x00,	0x00
	};

	writeRegisters(g_320x200x256);
	return true;
}

void VideoGraphicsArray::
putPixel( uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b )
{
	putPixel( x, y, getColorIndex(r,g,b) );
}

