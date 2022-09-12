#include <hardwarecommunication/port.h>
using namespace myos::common;
using namespace myos::hardwarecommunication;

// Base
// -----------------

Port::
Port( uint16_t portnumber )
{
	_portnumber = portnumber;
}

Port::
~Port()
{
}

// 8 bits
// -----------------

Port8bit::
Port8bit( uint16_t portnumber )
:Port( portnumber )
{

}

Port8bit::
~Port8bit()
{

}

void Port8bit::
write( uint8_t data )
{
	asm volatile( "outb %0, %1": : "a" (data), "Nd" (_portnumber) );
}

uint8_t Port8bit::
read()
{
	uint8_t result;
	asm volatile( "inb %1, %0": "=a" (result) : "Nd" (_portnumber) );
	return result;
}

// 8 bits Slow
// -----------------

Port8bitSlow::
Port8bitSlow( uint16_t portnumber )
:Port8bit( portnumber )
{

}

Port8bitSlow::
~Port8bitSlow()
{

}

void Port8bitSlow::
write( uint8_t data )
{
        asm volatile( "outb %0, %1\njmp 1f\njmp 1f\n1:": 
		     : "a" (data), "Nd" (_portnumber) );
}



// 16 bits
// -----------------

Port16bit::
Port16bit( uint16_t portnumber )
:Port( portnumber )
{

}

Port16bit::
~Port16bit()
{

}

void Port16bit::
write( uint16_t data )
{
        asm volatile( "outw %0, %1": : "a" (data), "Nd" (_portnumber) );
}

uint16_t Port16bit::
read()
{
        uint16_t result;
        asm volatile( "inw %1, %0": "=a" (result) : "Nd" (_portnumber) );
        return result;
}

// 32 bits
// --------------------

Port32bit::
Port32bit( uint16_t portnumber )
:Port( portnumber )
{

}

Port32bit::
~Port32bit()
{

}

void Port32bit::
write( uint32_t data )
{
        asm volatile( "outl %0, %1": : "a" (data), "Nd" (_portnumber) );
}

uint32_t Port32bit::
read()
{
        uint32_t result;
        asm volatile( "inl %1, %0": "=a" (result) : "Nd" (_portnumber) );
        return result;
}

