#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <gui/desktop.h>
#include <gui/window.h>
using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
//#define USEGUI
#ifdef USEGUI
using namespace myos::gui;
#endif

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

void printfHex( uint8_t key )
{
	static char* keytxt = "00 ";
    static char* hex = "0123456789ABCDEF";
    keytxt[0] = hex[(key >> 4) & 0xF];
    keytxt[1] = hex[(key >> 0) & 0xF];
    printf( keytxt );
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
	void onKeyDown( int8_t c ) override 
	{
		int8_t s[2];
		s[0] = c;
		s[1] = '\0';
		printf( s );
	}
};

class MouseToConsole : public MouseEventHandler
{
	int8_t _x;
	int8_t _y;
public:
	MouseToConsole()	
	{
		_x = 40;
		_y = 12;
		static uint16_t* VideoMemory = (uint16_t*)0xB8000;

	    VideoMemory[80*_y + _x] = ((VideoMemory[80*_y + _x] & 0xF000) >> 4) |
                                  ((VideoMemory[80*_y + _x] & 0x0F00) << 4) |
                                  ((VideoMemory[80*_y + _x] & 0x00FF)     );

	}

	void onMouseMove( int32_t xoff, int32_t yoff  ) override 
	{
		static uint16_t* VideoMemory = (uint16_t*)0xB8000;

        VideoMemory[80*_y + _x] = ((VideoMemory[80*_y + _x] & 0xF000) >> 4) |
                                  ((VideoMemory[80*_y + _x] & 0x0F00) << 4) |
                                  ((VideoMemory[80*_y + _x] & 0x00FF)     );
		
		_x += xoff;
        if( _x < 0) _x = 0;
        if( _x > 79) _x = 79;
		_y += yoff;
        if( _y < 0) _y = 0;
        if( _y > 24) _y = 24;

        VideoMemory[80*_y + _x] = ((VideoMemory[80*_y + _x] & 0xF000) >> 4) |
                                  ((VideoMemory[80*_y + _x] & 0x0F00) << 4) |
                                  ((VideoMemory[80*_y + _x] & 0x00FF)     );
	}
};

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
	for( constructor i = start_ctors; i != end_ctors; i++ )
		(*i)();
}

void taskA()
{
	while( true )
		printf("A");
}

void taskB()
{
	while( true )
		printf("B");	
}

extern "C" void kernelMain( void* multiboot_structure, uint32_t magicnumber )
{
	printf( "Hello World! --- http://www.AlgorithMan.de\n" );

	GlobalDescriptorTable gdt;

	TaskManager taskManager;
	Task task1( &gdt, taskA );
	Task task2( &gdt, taskB );
	taskManager.addTask( &task1 );
	taskManager.addTask( &task2 );
	
	InterruptManager interrupts( &gdt, &taskManager );

	printf( "Initializing Hardware, Stage 1\n" );

#ifdef USEGUI
	Desktop desktop( 320, 200, 0x00, 0x00, 0xA8);
#endif
	
	DriverManager drvManager;

#ifndef USEGUI
	PrintfKeyboardEventHandler kbhandler;
	KeyboardDriver keyboard( &interrupts, &kbhandler );
#else
	KeyboardDriver keyboard( &interrupts, &desktop );
#endif
	drvManager.addDriver( &keyboard );

#ifndef USEGUI
	MouseToConsole mousehandler; 
	MouseDriver mouse( &interrupts, &mousehandler );
#else
	MouseDriver mouse( &interrupts, &desktop );
#endif
	drvManager.addDriver( &mouse );

	PeripheralComponentInterconnectController PCIController;
	PCIController.selectDriver( &drvManager, &interrupts );
#ifdef USEGUI
	VideoGraphicsArray vga;
#endif
	printf( "Initializing Hardware, Stage 2\n" );

	drvManager.activateAll();	

	printf( "Initializing Hardware, Stage 3\n" );

#ifdef USEGUI
	vga.setMode( 320, 200, 8 );
	
	Window win1( &desktop, 10, 10, 20, 20, 0xA8, 0x00, 0x00 );
	desktop.addChild( &win1 );
	Window win2( &desktop, 190, 50, 30, 30, 0x00, 0xA8, 0x00 );
	desktop.addChild( &win2 );
#endif
	
	interrupts.activate();

	while(1){
#ifdef USEGUI
		desktop.draw( &vga );
#endif
	}
}
