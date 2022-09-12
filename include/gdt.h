#ifndef __MYOS__GDT_H
#define __MYOS__GDT_H

#include <common/types.h>

namespace myos
{
	class GlobalDescriptorTable
	{
	public:
		class SegmentDescriptor
		{
		private:
			myos::common::uint16_t _limit_lo;
			myos::common::uint16_t _base_lo;
			myos::common::uint8_t _base_hi;
			myos::common::uint8_t _type;
			myos::common::uint8_t _flags_limit_hi;
			myos::common::uint8_t _base_vhi;
		public:
			SegmentDescriptor( myos::common::uint32_t base, myos::common::uint32_t limit, myos::common::uint8_t type );
			myos::common::uint32_t base();
			myos::common::uint32_t limit();
		} __attribute__((packed));

		SegmentDescriptor _nullSegmentDescriptor;
		SegmentDescriptor _unusedSegmentDescriptor;
		SegmentDescriptor _codeSegmentDescriptor;
		SegmentDescriptor _dataSegmentDescriptor;

	public:
		GlobalDescriptorTable();
		~GlobalDescriptorTable();

		myos::common::uint16_t codeSegmentSelector();
		myos::common::uint16_t dataSegmentSelector();	
	};
}

#endif //__MYOS__GDT_H
