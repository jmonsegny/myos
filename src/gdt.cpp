#include <gdt.h>
using namespace myos;
using namespace myos::common;

GlobalDescriptorTable::
GlobalDescriptorTable()
:_nullSegmentDescriptor(0,0,0),
 _unusedSegmentDescriptor(0,0,0),
 _codeSegmentDescriptor(0,64*1024*1024,0x9A), // 64 Mb 
 _dataSegmentDescriptor(0,64*1024*1024,0x92)  // 64 Mb
{
	uint32_t i[2];
	i[1] = (uint32_t)this; // 32 bits because it's a 32 bit OS			       
	i[0] = sizeof(GlobalDescriptorTable) << 16;

	asm volatile( "lgdt (%0)": :"p" (((uint8_t *)i)+2) );
}

GlobalDescriptorTable::
~GlobalDescriptorTable()
{
}

uint16_t GlobalDescriptorTable::
dataSegmentSelector()
{
	return (uint8_t*)&_dataSegmentDescriptor - (uint8_t*)this; 
	// 8 bits because it's relative to the start of the GDT
}

uint16_t GlobalDescriptorTable::
codeSegmentSelector()
{
        return (uint8_t*)&_codeSegmentDescriptor - (uint8_t*)this;
}

GlobalDescriptorTable::SegmentDescriptor::
SegmentDescriptor( uint32_t base, uint32_t limit, uint8_t flags )
{
	uint8_t* target = (uint8_t*)this;

	if( limit < 655235 ) {
		target[6] = 0x40;
	} else {
		if( (limit & 0xFFF) != 0xFFF)
			limit = (limit >> 12) - 1;
		else
			limit = (limit >> 12);
		target[6] = 0xC0;
	}

	target[0] = limit & 0xFF;
	target[1] = (limit >> 8) & 0xFF;
	target[6] |= (limit >> 16) & 0xF;
	target[2] = (base >> 0) & 0xFF;
	target[3] = (base >> 8) & 0xFF;
	target[4] = (base >> 16) & 0xFF;
	target[7] = (base >> 24) & 0xFF;
	target[5] = flags;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::
base()
{
	uint8_t* target = (uint8_t*)this;
	uint32_t result = target[7];
	result = (result << 8) + target[4];
	result = (result << 8) + target[3];
	result = (result << 8) + target[2];
	return result;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::
limit()
{
	uint8_t* target = (uint8_t*)this;
	uint32_t result = target[6] & 0xF;
	result = (result << 8) + target[1];
	result = (result << 8) + target[0];
	if( (target[6] & 0xF0) == 0xC0 )
		result = (result << 12) | 0xFFF;
	return result;
}
