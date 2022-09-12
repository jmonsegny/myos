#include "mouse.h"

void printf( int8_t* );

MouseEventHandler::
MouseEventHandler()
{
}

void MouseEventHandler::
onMouseDown( uint8_t button)
{
}

void MouseEventHandler::
onMouseUp( uint8_t button )
{
}

void MouseEventHandler::
onMouseMove( int8_t xoff, int8_t yoff )
{
}

void MouseEventHandler::
onActivate( )
{
}


MouseDriver::
MouseDriver( InterruptManager* manager, MouseEventHandler* handler )
:InterruptHandler(0x2C,manager),
dataport(0x60),
commandport(0x64)
{
	_handler = handler;
}

MouseDriver::
~MouseDriver()
{
}

void MouseDriver::
activate()
{
	offset = 0;
    buttons = 0;

    commandport.Write(0xAB); // activate
    commandport.Write(0x20); // get curr state

    uint8_t status = dataport.Read() | 2; // flip
    commandport.Write(0x60); // set state
    dataport.Write(status);

    commandport.Write(0xD4);
    dataport.Write(0xF4);
    dataport.Read();
}

uint32_t MouseDriver::
HandleInterrupt( uint32_t esp )
{
	//static uint8_t respond = 0;
	uint8_t status = commandport.Read();

	if( !(status & 0x20) )
		return	esp;

	buffer[offset] = dataport.Read();
	offset = (offset + 1) % 3;

	if(  _handler == 0 )
        return  esp;

	if( !(status & 0x20) || _handler == 0 )
        return  esp;

	if( offset == 0 ) {

		if( buffer[1] != 0 || buffer[2] != 0 ) {
			_handler->onMouseMove( buffer[1], -buffer[2] );		
		}

		for( uint8_t i = 0; i < 3; i++) {
			
			if( (buffer[0] & (0x01 << i)) != (buttons & (0x01 << i))) {
				
				if( buttons & (0x1 << 1) )
					_handler->onMouseUp(i+1);
				else
					_handler->onMouseDown(i+1);
			}
		}
		buttons = buffer[0];
	}

	return esp;
}
