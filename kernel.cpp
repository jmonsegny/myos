#include "types.h"
#include "gdt.h"
#include "interrupts.h"
#include "keyboard.h"

void printf( int8_t* str )
{
	static uint16_t* VideoMemory = (uint16_t*)0xb8000;

	static uint8_t x = 0, y = 0;

	for( int32_t i = 0; str[i] != '\0'; i++ ) {
		if( str[i] == '\n' ) {
			y++;
			x = 0;			
		} else {			
			VideoMemory[y*80+x] = (VideoMemory[y*80+x] & 0xFF00) | str[i];
			x++;
		}

		if( x >= 80 ) {
			y++;
			x = 0;
		}

		if( y >= 25 ) {
			for( int j = 0; j < 25*80; j++ )
				VideoMemory[j] = (VideoMemory[i] & 0xFF00) | ' ';
			x = 0;
			y = 0;
		}
	}
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
	for( constructor i = start_ctors; i != end_ctors; i++ )
		(*i)();
}

extern "C" void kernelMain( void* multiboot_structure, uint32_t magicnumber )
{
	printf( "Hello World! --- http://www.AlgorithMan.de\n" );
	printf( "Hello World! --- http://www.AlgorithMan.de\n" );

	GlobalDescriptorTable gdt;
	InterruptManager interrupts( &gdt );

	KeyboardDriver keyboard( &interrupts );

	interrupts.Activate();

	while(1);
}
