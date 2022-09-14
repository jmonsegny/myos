#ifndef __MYOS__HARDWARECOMMUNICATION__PCI_H
#define __MYOS__HARDWARECOMMUNICATION__PCI_H

#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
#include <common/types.h>
#include <hardwarecommunication/interrupts.h>

namespace myos
{
	namespace hardwarecommunication
	{

		enum BaseAddressRegisterType
	    {
    	    MemoryMapping = 0,
	        InputOutput = 1
    	};

	    class BaseAddressRegister
    	{
	    public:
    	    bool prefetchable;
        	myos::common::uint8_t* address;
	        myos::common::uint32_t size;
    	    BaseAddressRegisterType type;
	    };

		class PeripheralComponentInterconnectDeviceDescriptor
		{
		public:
			myos::common::uint32_t _portBase;
			myos::common::uint32_t _interrupt;
			myos::common::uint16_t _bus;
			myos::common::uint16_t _device;
			myos::common::uint16_t _function;
            myos::common::uint16_t _vendor_id;
            myos::common::uint16_t _device_id;
            myos::common::uint8_t _class_id;
            myos::common::uint8_t _subclass_id;
            myos::common::uint8_t _interface_id;
            myos::common::uint8_t _revision;

//			PeripheralComponentInterconnectDeviceDescriptor();
//			~PeripheralComponentInterconnectDeviceDescriptor();
		};

		class PeripheralComponentInterconnectController
		{
			Port32bit _dataPort;
			Port32bit _commandPort;
		public:
			PeripheralComponentInterconnectController();
			~PeripheralComponentInterconnectController();

			myos::common::uint32_t read( myos::common::uint16_t bus, 
                                         myos::common::uint16_t device, 
			                             myos::common::uint16_t function,
			                             myos::common::uint32_t registeroffset );

			void write( myos::common::uint16_t bus, 
                        myos::common::uint16_t device, 
                        myos::common::uint16_t function, 
                        myos::common::uint32_t registeroffset,
		                myos::common::uint32_t value );

			bool deviceHasFunctions( myos::common::uint16_t bus,
                                     myos::common::uint16_t device );

			void selectDriver( myos::drivers::DriverManager* driverManager, 
			                   myos::hardwarecommunication::InterruptManager* interrupts );

			myos::drivers::Driver* getDriver( PeripheralComponentInterconnectDeviceDescriptor,
			                                  myos::hardwarecommunication::InterruptManager* interrupts );
			
			PeripheralComponentInterconnectDeviceDescriptor 
			        getDeviceDescriptor( myos::common::uint16_t bus,
                                         myos::common::uint16_t device,
                                         myos::common::uint16_t function );

			BaseAddressRegister getBaseAddressRegister( myos::common::uint16_t bus,
                                                        myos::common::uint16_t device,
                                                        myos::common::uint16_t function,
			                                            myos::common::uint16_t bar );
		};	
	}
}

#endif // __MYOS__HARDWARECOMMUNICATION__PCI_H
