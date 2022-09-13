#include <hardwarecommunication/pci.h>
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;

void printf( int8_t* s );
void printfHex( uint8_t s );

PeripheralComponentInterconnectController::
PeripheralComponentInterconnectController()
:_dataPort(0xCFC),
_commandPort(0xCF8)
{
}

PeripheralComponentInterconnectController::
~PeripheralComponentInterconnectController()
{
}

uint32_t PeripheralComponentInterconnectController::
read( uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset )
{
	uint32_t id = ((0x1                  ) << 31) |
	              ((bus &            0xFF) << 16) |
	              ((device &         0x1F) << 11) |
	              ((function &       0x07) << 8)  |
	              ((registeroffset & 0xFC) << 0 );

	_commandPort.write( id );
	uint32_t result = _dataPort.read();
	return result >> ( 8*(registeroffset % 4) );
}

void PeripheralComponentInterconnectController::
write( uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value )
{
	uint32_t id = ((0x1                  ) << 31) |
                  ((bus &            0xFF) << 16) |
                  ((device &         0x1F) << 11) |
                  ((function &       0x07) << 8)  |
                  ((registeroffset & 0xFC) << 0 );
	_commandPort.write( id );
	_dataPort.write( value );
}

bool PeripheralComponentInterconnectController::
deviceHasFunctions( uint16_t bus, uint16_t device )
{
	return read( bus, device, 0, 0x0E ) & ( 1 << 7 );
}

void PeripheralComponentInterconnectController::
selectDriver( DriverManager* driverManager )
{
	for( uint16_t bus = 0; bus < 8; bus++ ) {
		for( uint16_t dev = 0; dev < 32; dev++ ) {
			int numFunctions = deviceHasFunctions( bus, dev )? 8:1;
			for( int fun = 0; fun < numFunctions; fun++) {
				PeripheralComponentInterconnectDeviceDescriptor device = getDeviceDescriptor( bus, dev, fun );

				if( device._vendor_id == 0x0000 || device._vendor_id == 0xFFFF )
					break;

				printf( "PCI BUS " );
				printfHex( bus & 0xFF);
				printf( ", DEVICE " );
				printfHex( dev & 0xFF );
				printf( ", FUNCTION " );
				printfHex( fun & 0xFF );
				printf( " = VENDOR " );
				printfHex( (device._vendor_id & 0xFF00) >> 8 );
				printfHex(  device._vendor_id & 0x00FF       );
				printf( ", DEVICE " );
				printfHex( (device._device_id & 0xFF00) >> 8 );
				printfHex(  device._device_id & 0x00FF       );
				printf( "\n" );
			}
		} 
	}
}

PeripheralComponentInterconnectDeviceDescriptor PeripheralComponentInterconnectController::
getDeviceDescriptor( uint16_t bus, uint16_t device, uint16_t function )
{
	PeripheralComponentInterconnectDeviceDescriptor result;

	result._bus = bus;
	result._device = device;
	result._function = function;

	result._vendor_id =    read( bus, device, function, 0x00 );
    result._device_id =    read( bus, device, function, 0x02 );
    result._subclass_id =  read( bus, device, function, 0x0B );
    result._interface_id = read( bus, device, function, 0x0A );
    result._revision =     read( bus, device, function, 0x09 );
    result._interrupt =    read( bus, device, function, 0x3C );

	return result;
}
