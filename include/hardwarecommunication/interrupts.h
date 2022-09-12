#ifndef __MYOS__HARDWARECOMMUNICATION__INTERRUPTS_H
#define __MYOS__HARDWARECOMMUNICATION__INTERRUPTS_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <gdt.h>

namespace myos
{
	namespace hardwarecommunication
	{

		class InterruptHandler;

		class InterruptManager
		{
			friend class InterruptHandler;
		protected:

			static InterruptManager* _activeInterruptManager;
			InterruptHandler* _handlers[256];

			struct GateDescriptor
			{
				myos::common::uint16_t handlerAddressLowBits;
				myos::common::uint16_t gdt_codeSegmentSelector;
				myos::common::uint8_t reserved;
				myos::common::uint8_t access;
				myos::common::uint16_t handlerAddressHighBits;
			} __attribute__((packed));

			static GateDescriptor _interruptDescriptorTable[256];

			struct InterruptDescriptorTablePointer
			{
				myos::common::uint16_t size;
				myos::common::uint32_t base;
			} __attribute__((packed));

			static void setInterruptDescriptorTableEntry( 
					myos::common::uint8_t interruptNumber,
					myos::common::uint16_t codeSegmentSelectorOffset,
					void (*handler)(),
					myos::common::uint8_t DescriptorPrivilegeLevel,
					myos::common::uint8_t DescriptorType
					);

			Port8bitSlow _picMasterCommand;
			Port8bitSlow _picMasterData;
			Port8bitSlow _picSlaveCommand;
	        Port8bitSlow _picSlaveData;

		public:
			InterruptManager( myos::GlobalDescriptorTable *gdt );
			~InterruptManager();	

			void activate();
			void deactivate();

			static myos::common::uint32_t handleInterrupt( myos::common::uint8_t interruptNumber, myos::common::uint32_t esp );	
			myos::common::uint32_t doHandleInterrupt( myos::common::uint8_t interruptNumber, myos::common::uint32_t esp );

			static void interruptIgnore();
			static void handleInterruptRequest0x00();
			static void handleInterruptRequest0x01();
			static void handleInterruptRequest0x02();
			static void handleInterruptRequest0x03();
			static void handleInterruptRequest0x04();
			static void handleInterruptRequest0x05();
			static void handleInterruptRequest0x06();
			static void handleInterruptRequest0x07();
			static void handleInterruptRequest0x08();
			static void handleInterruptRequest0x09();
			static void handleInterruptRequest0x0A();
			static void handleInterruptRequest0x0B();
			static void handleInterruptRequest0x0C();
			static void handleInterruptRequest0x0D();
			static void handleInterruptRequest0x0E();
			static void handleInterruptRequest0x0F();
			static void handleInterruptRequest0x31();

			static void handleException0x00();
			static void handleException0x01();
			static void handleException0x02();
            static void handleException0x03();
            static void handleException0x04();
            static void handleException0x05();
            static void handleException0x06();
            static void handleException0x07();
            static void handleException0x08();
            static void handleException0x09();
            static void handleException0x0A();
            static void handleException0x0B();
            static void handleException0x0C();
            static void handleException0x0D();
            static void handleException0x0E();
            static void handleException0x0F();
            static void handleException0x10();
            static void handleException0x11();
            static void handleException0x12();
            static void handleException0x13();
		};

		class InterruptHandler
		{
		protected:
        		myos::common::uint8_t _interruptNumber;
		        InterruptManager* _interruptManager;

        		InterruptHandler( myos::common::uint8_t interruptNumber, InterruptManager* interruptManager );
		        ~InterruptHandler();
		public:
        		virtual myos::common::uint32_t handleInterrupt( myos::common::uint32_t esp );
		};
	}
}


#endif // __MYOS__HARDWARECOMMUNICATION__INTERRUPTS_H
