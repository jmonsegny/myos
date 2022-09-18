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
selectDriver( DriverManager* driverManager, InterruptManager* interrupts )
{
    for( uint16_t bus = 0; bus < 8; bus++ ) {
        for( uint16_t dev = 0; dev < 32; dev++ ) {
            int numFunctions = deviceHasFunctions( bus, dev )? 8:1;
            for( int fun = 0; fun < numFunctions; fun++) {
                PeripheralComponentInterconnectDeviceDescriptor device = getDeviceDescriptor( bus, dev, fun );

                if( device._vendor_id == 0x0000 || device._vendor_id == 0xFFFF )
                    continue;

                for( int barnum = 0; barnum < 6; barnum++ ) {
                    BaseAddressRegister bar = getBaseAddressRegister( bus, dev, fun, barnum );
                    if( bar.address && (bar.type == InputOutput) )
                        device._portBase = (uint32_t)bar.address;

                    Driver* driver = getDriver( device, interrupts );
                    if( driver != 0 )
                        driverManager->addDriver( driver );
                }

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

Driver* PeripheralComponentInterconnectController::
getDriver( PeripheralComponentInterconnectDeviceDescriptor dev,
           InterruptManager* interrupts )
{
    switch( dev._vendor_id )
    {
    case 0x1022: //AMD
        switch( dev._device_id )
        {
        case 0x2000: // amd79c973
            break;
        }
        break;
    case 0x8086: // Intel
        break;
    }

    switch( dev._class_id )
    {
    case 0x03: // Graphics
        switch( dev._subclass_id )
        {
        case 0x00: // VGA
            break;
        }
        break;
    }

    return 0;
}

BaseAddressRegister PeripheralComponentInterconnectController::
getBaseAddressRegister( uint16_t bus, uint16_t device, uint16_t function, uint16_t bar )
{
    BaseAddressRegister result;

    uint32_t headertype = read( bus, device, function, 0x0E ) & 0x7F;
    int32_t maxBARs = 6 - (4*headertype);
    if( bar >= maxBARs )
        return result;

    uint32_t bar_value = read( bus, device, function, 0x10 + 4*bar );
    result.type = (bar_value & 0x1) ? InputOutput: MemoryMapping;
    uint32_t temp;

    if( result.type == MemoryMapping ) {
        // TODO: COMPLETE LATER. See lowlevel.eu PCI
        switch((bar_value >> 1) &  0x3) {
        case 0x00: // 32 bits
            break;
        case 0x01: // 20 bits
            break;
        case 0x10: // 64 bits
            break;
        }
    } else {
        result.address = (uint8_t*)( bar_value & ~0x3 );
        result.prefetchable = false;
    }

    return result;
}
