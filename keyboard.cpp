#include "keyboard.h"

void printf( int8_t* );

KeyboardDriver::
KeyboardDriver( InterruptManager* manager )
:InterruptHandler(0x21,manager),
dataport(0x60),
commandport(0x64)	
{
	while( commandport.Read() & 0x1 ) {
		dataport.Read();
	}
	commandport.Write(0xAE); // activate
	commandport.Write(0x20); // get curr state

	uint8_t status = (dataport.Read() | 1) & ~0x10; // flip
	commandport.Write(0x60); // set state
	dataport.Write(status);

	dataport.Write(0xF4);
}

KeyboardDriver::
~KeyboardDriver()
{
}

uint32_t KeyboardDriver::
HandleInterrupt( uint32_t esp )
{
	uint8_t key = dataport.Read();
	static char* keytxt = "KEYBOARD 0x00 ";
        static char* hex = "0123456789ABCDEF";
        keytxt[11] = hex[(key >> 4) & 0xF];
        keytxt[12] = hex[(key >> 0) & 0xF];
        printf( keytxt );
	return esp;
}
