#include <hardwarecommunication/interrupts.h>
using namespace myos::common;
using namespace myos::hardwarecommunication;
using namespace myos;

void printf( int8_t* s );
void printfHex( uint8_t s );

InterruptHandler::
InterruptHandler( uint8_t interruptNumber, InterruptManager* interruptManager )
{
	_interruptNumber = interruptNumber;
	_interruptManager = interruptManager;
	_interruptManager->_handlers[interruptNumber] = this;
}

InterruptHandler::
~InterruptHandler()
{
	if( _interruptManager->_handlers[_interruptNumber] == this )
		_interruptManager->_handlers[_interruptNumber] = 0;
}

uint32_t InterruptHandler::
handleInterrupt( uint32_t esp )
{
	return esp;
}

InterruptManager::GateDescriptor InterruptManager::_interruptDescriptorTable[256];

InterruptManager* InterruptManager::_activeInterruptManager = 0;

void InterruptManager::
setInterruptDescriptorTableEntry(
                        uint8_t interruptNumber,
                        uint16_t codeSegmentSelectorOffset,
                        void (*handler)(),
                        uint8_t DescriptorPrivilegeLevel,
                        uint8_t DescriptorType
                        )
{
	const uint8_t IDT_DESC_PRESENT = 0x80;

	_interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;;
	_interruptDescriptorTable[interruptNumber].handlerAddressHighBits = (((uint32_t)handler) >> 16) & 0xFFFF;
	_interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
	_interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrivilegeLevel & 3) << 5);
	_interruptDescriptorTable[interruptNumber].reserved = 0;
}

InterruptManager::
InterruptManager( GlobalDescriptorTable *gdt )
:_picMasterCommand(0x20),
_picMasterData(0x21),
_picSlaveCommand(0xA0),
_picSlaveData(0xA1)
{
	uint16_t codeSegment = gdt->codeSegmentSelector();
	const uint8_t IDT_INTERRUPT_GATE = 0xE;

	for( uint16_t i = 0; i < 256; i++ ) {
		_handlers[i] = 0;
		setInterruptDescriptorTableEntry(i, codeSegment, &interruptIgnore, 0, IDT_INTERRUPT_GATE );
	}

	setInterruptDescriptorTableEntry(0x00, codeSegment, &handleException0x00, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(0x01, codeSegment, &handleException0x01, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(0x02, codeSegment, &handleException0x02, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(0x03, codeSegment, &handleException0x03, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(0x04, codeSegment, &handleException0x04, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(0x05, codeSegment, &handleException0x05, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(0x06, codeSegment, &handleException0x06, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(0x07, codeSegment, &handleException0x07, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(0x08, codeSegment, &handleException0x08, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(0x09, codeSegment, &handleException0x09, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(0x0A, codeSegment, &handleException0x0A, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(0x0B, codeSegment, &handleException0x0B, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(0x0C, codeSegment, &handleException0x0C, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(0x0D, codeSegment, &handleException0x0D, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(0x0E, codeSegment, &handleException0x0E, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(0x0F, codeSegment, &handleException0x0F, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(0x10, codeSegment, &handleException0x10, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(0x11, codeSegment, &handleException0x11, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(0x12, codeSegment, &handleException0x12, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(0x13, codeSegment, &handleException0x13, 0, IDT_INTERRUPT_GATE );

	int16_t hardwareInterruptOffset = 0x20;
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x00, codeSegment, &handleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE );
	setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x01, codeSegment, &handleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE );
	setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x02, codeSegment, &handleInterruptRequest0x02, 0, IDT_INTERRUPT_GATE );
	setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x03, codeSegment, &handleInterruptRequest0x03, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x04, codeSegment, &handleInterruptRequest0x04, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x05, codeSegment, &handleInterruptRequest0x05, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x06, codeSegment, &handleInterruptRequest0x06, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x07, codeSegment, &handleInterruptRequest0x07, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x08, codeSegment, &handleInterruptRequest0x08, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x09, codeSegment, &handleInterruptRequest0x09, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0A, codeSegment, &handleInterruptRequest0x0A, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0B, codeSegment, &handleInterruptRequest0x0B, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0C, codeSegment, &handleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0D, codeSegment, &handleInterruptRequest0x0D, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0E, codeSegment, &handleInterruptRequest0x0E, 0, IDT_INTERRUPT_GATE );
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0F, codeSegment, &handleInterruptRequest0x0F, 0, IDT_INTERRUPT_GATE );

	_picMasterCommand.write(0x11);
	_picSlaveCommand.write(0x11);

	_picMasterData.write(0x20);
    _picSlaveData.write(0x28);

	_picMasterData.write(0x04);
    _picSlaveData.write(0x02);

	_picMasterData.write(0x01);
    _picSlaveData.write(0x01);

	_picMasterData.write(0x00);
    _picSlaveData.write(0x00);

	InterruptDescriptorTablePointer idt;
	idt.size = 256 * sizeof(GateDescriptor) - 1;
	idt.base = (uint32_t)_interruptDescriptorTable;
	asm volatile("lidt %0" : : "m" (idt));
}

InterruptManager::        
~InterruptManager()
{
}

void InterruptManager::
activate()
{
	if( _activeInterruptManager != 0 )
		_activeInterruptManager->deactivate();
	_activeInterruptManager = this;
	asm ( "sti" );
}

void InterruptManager::
deactivate()
{
	if( _activeInterruptManager == this ) {
		_activeInterruptManager = 0;
		asm( "cli" );
	}		
}

uint32_t InterruptManager::
handleInterrupt( uint8_t interruptNumber, uint32_t esp )
{
	if( _activeInterruptManager != 0 )
		return _activeInterruptManager->doHandleInterrupt( interruptNumber, esp );

	return esp;
}

uint32_t InterruptManager::
doHandleInterrupt( uint8_t interruptNumber, uint32_t esp )
{
	if( _handlers[interruptNumber] != 0 ) {
		esp = _handlers[interruptNumber]->handleInterrupt( esp );
	}
	else if( interruptNumber != 0x20 ) {
		printf( "UNHANDLED INTERRUPT 0x" );
		printfHex( interruptNumber );
		printf( " " );	
	}

	if( 0x20 <= interruptNumber && interruptNumber < 0x30 ) {
		_picMasterCommand.write(0x20);
		if( 0x28 <= interruptNumber )
			_picSlaveCommand.write(0x20);
	}

	return esp;
}
