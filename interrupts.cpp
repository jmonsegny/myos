#include "interrupts.h"

void printf( int8_t* s );

uint32_t InterruptManager::handleInterrupt( uint8_t interruptNumber, uint32_t esp )
{
	printf("INTERRUPT");

	return esp;
}

//void InterruptManager::HandleInterruptRequest0x00(){}
