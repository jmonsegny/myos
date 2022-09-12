#include <drivers/keyboard.h>
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;

void printf( int8_t* );
void printfHex( uint8_t );

KeyboardEventHandler::
KeyboardEventHandler()
{
}

void KeyboardEventHandler::
onKeyDown( int8_t c)
{
}

void KeyboardEventHandler::
onKeyUp( int8_t c )
{
}

KeyboardDriver::
KeyboardDriver( InterruptManager* manager, KeyboardEventHandler* handler )
:InterruptHandler(0x21,manager),
_dataport(0x60),
_commandport(0x64)	
{
	_handler = handler;
}

KeyboardDriver::
~KeyboardDriver()
{
}

void KeyboardDriver::
activate()
{
	while( _commandport.read() & 0x1 ) {
        _dataport.read();
    }
    _commandport.write(0xAE); // activate
    _commandport.write(0x20); // get curr state

    uint8_t status = (_dataport.read() | 1) & ~0x10; // flip
    _commandport.write(0x60); // set state
    _dataport.write(status);

    _dataport.write(0xF4);
}

uint32_t KeyboardDriver::
handleInterrupt( uint32_t esp )
{
	uint8_t key = _dataport.read();

	if( _handler == 0 )
		return esp;

	static bool Shift = false;

	//if( key < 0x80 ) {
	
		switch( key ) {
			case 0x45:
			case 0xC5:
			case 0xFA:
				break;
			case 0x02: if( Shift ) _handler->onKeyDown('!'); else _handler->onKeyDown('1'); break;
			case 0x03: if( Shift ) _handler->onKeyDown('@'); else _handler->onKeyDown('2'); break;
			case 0x04: if( Shift ) _handler->onKeyDown('#'); else _handler->onKeyDown('3'); break;
			case 0x05: if( Shift ) _handler->onKeyDown('$'); else _handler->onKeyDown('4'); break;
			case 0x06: if( Shift ) _handler->onKeyDown('%'); else _handler->onKeyDown('5'); break;
			case 0x07: if( Shift ) _handler->onKeyDown('^'); else _handler->onKeyDown('6'); break;
			case 0x08: if( Shift ) _handler->onKeyDown('&'); else _handler->onKeyDown('7'); break;
			case 0x09: if( Shift ) _handler->onKeyDown('*'); else _handler->onKeyDown('8'); break;
			case 0x0A: if( Shift ) _handler->onKeyDown('('); else _handler->onKeyDown('9'); break;
		    case 0x0B: if( Shift ) _handler->onKeyDown(')'); else _handler->onKeyDown('0'); break;
		    case 0x0C: if( Shift ) _handler->onKeyDown('_'); else _handler->onKeyDown('-'); break;
			case 0x0D: if( Shift ) _handler->onKeyDown('+'); else _handler->onKeyDown('='); break;
			case 0x10: if( Shift ) _handler->onKeyDown('Q'); else _handler->onKeyDown('q'); break;				
			case 0x11: if( Shift ) _handler->onKeyDown('W'); else _handler->onKeyDown('w'); break;
			case 0x12: if( Shift ) _handler->onKeyDown('E'); else _handler->onKeyDown('e'); break;
			case 0x13: if( Shift ) _handler->onKeyDown('R'); else _handler->onKeyDown('r'); break;
			case 0x14: if( Shift ) _handler->onKeyDown('T'); else _handler->onKeyDown('t'); break;
			case 0x15: if( Shift ) _handler->onKeyDown('Y'); else _handler->onKeyDown('y'); break;
			case 0x16: if( Shift ) _handler->onKeyDown('U'); else _handler->onKeyDown('u'); break;
			case 0x17: if( Shift ) _handler->onKeyDown('I'); else _handler->onKeyDown('i'); break;
			case 0x18: if( Shift ) _handler->onKeyDown('O'); else _handler->onKeyDown('o'); break;
			case 0x19: if( Shift ) _handler->onKeyDown('P'); else _handler->onKeyDown('p'); break;
			case 0x1A: if( Shift ) _handler->onKeyDown('{'); else _handler->onKeyDown('['); break;
			case 0x1B: if( Shift ) _handler->onKeyDown('}'); else _handler->onKeyDown(']'); break;
			case 0x1E: if( Shift ) _handler->onKeyDown('A'); else _handler->onKeyDown('a'); break;
			case 0x1F: if( Shift ) _handler->onKeyDown('S'); else _handler->onKeyDown('s'); break;
		    case 0x20: if( Shift ) _handler->onKeyDown('D'); else _handler->onKeyDown('d'); break;
			case 0x21: if( Shift ) _handler->onKeyDown('F'); else _handler->onKeyDown('f'); break;
			case 0x22: if( Shift ) _handler->onKeyDown('G'); else _handler->onKeyDown('g'); break;
			case 0x23: if( Shift ) _handler->onKeyDown('H'); else _handler->onKeyDown('h'); break;
			case 0x24: if( Shift ) _handler->onKeyDown('J'); else _handler->onKeyDown('j'); break;
			case 0x25: if( Shift ) _handler->onKeyDown('K'); else _handler->onKeyDown('k'); break;
			case 0x26: if( Shift ) _handler->onKeyDown('L'); else _handler->onKeyDown('l'); break;
			case 0x27: if( Shift ) _handler->onKeyDown(':'); else _handler->onKeyDown(';'); break;
			case 0x28: if( Shift ) _handler->onKeyDown('\"'); else _handler->onKeyDown('\''); break;
		    case 0x29: if( Shift ) _handler->onKeyDown('~'); else _handler->onKeyDown('`'); break;
			case 0x2B: if( Shift ) _handler->onKeyDown('|'); else _handler->onKeyDown('\\'); break;	   
			case 0x2C: if( Shift ) _handler->onKeyDown('Z'); else _handler->onKeyDown('z'); break;
			case 0x2D: if( Shift ) _handler->onKeyDown('X'); else _handler->onKeyDown('x'); break;
			case 0x2E: if( Shift ) _handler->onKeyDown('C'); else _handler->onKeyDown('c'); break;
			case 0x2F: if( Shift ) _handler->onKeyDown('V'); else _handler->onKeyDown('v'); break;				
		    case 0x30: if( Shift ) _handler->onKeyDown('B'); else _handler->onKeyDown('b'); break;
			case 0x31: if( Shift ) _handler->onKeyDown('N'); else _handler->onKeyDown('n'); break;
			case 0x32: if( Shift ) _handler->onKeyDown('M'); else _handler->onKeyDown('m'); break;
			case 0x33: if( Shift ) _handler->onKeyDown('<'); else _handler->onKeyDown(','); break;
			case 0x34: if( Shift ) _handler->onKeyDown('>'); else _handler->onKeyDown('.'); break;
			case 0x35: if( Shift ) _handler->onKeyDown('?'); else _handler->onKeyDown('/'); break;
			case 0x1C: _handler->onKeyDown('\n'); break;
			case 0x39: _handler->onKeyDown(' '); break;
			case 0x2A: 
			case 0x36: Shift = true; break;
			case 0xAA:
			case 0xB6: Shift = false; break;	   
			default:
				if( key < 0x80 ) {
					printf( "KEYBOARD 0x" );
					printfHex( key );
					printf( " " );
				}
		}
	//}
	return esp;
}
