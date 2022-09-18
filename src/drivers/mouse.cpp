#include <drivers/mouse.h>
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;

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
onMouseMove( int32_t xoff, int32_t yoff )
{
}

void MouseEventHandler::
onActivate( )
{
}


MouseDriver::
MouseDriver( InterruptManager* manager, MouseEventHandler* handler )
    :InterruptHandler(0x2C,manager),
     _dataport(0x60),
     _commandport(0x64)
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
    _offset = 0;
    _buttons = 0;

    // C
    if( _handler != 0 )
        _handler->onActivate();
    // C

    _commandport.write(0xAB); // activate
    _commandport.write(0x20); // get curr state

    uint8_t status = _dataport.read() | 2; // flip
    _commandport.write(0x60); // set state
    _dataport.write(status);

    _commandport.write(0xD4);
    _dataport.write(0xF4);
    _dataport.read();
}

uint32_t MouseDriver::
handleInterrupt( uint32_t esp )
{
    //static uint8_t respond = 0;
    uint8_t status = _commandport.read();

    if( !(status & 0x20) )
        return	esp;

    _buffer[_offset] = _dataport.read();
    _offset = (_offset + 1) % 3;

    if(  _handler == 0 )
        return  esp;

    if( !(status & 0x20) || _handler == 0 )
        return  esp;

    if( _offset == 0 ) {

        if( _buffer[1] != 0 || _buffer[2] != 0 ) {
            _handler->onMouseMove( ((int8_t)_buffer[1]), -((int8_t)_buffer[2]) );
        }

        for( uint8_t i = 0; i < 3; i++) {

            if( (_buffer[0] & (0x01 << i)) != (_buttons & (0x01 << i))) {

                if( _buttons & (0x1 << i) )
                    _handler->onMouseUp(i+1);
                else
                    _handler->onMouseDown(i+1);
            }
        }
        _buttons = _buffer[0];
    }

    return esp;
}

