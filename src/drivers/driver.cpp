#include <drivers/driver.h>
using namespace myos::common;
using namespace myos::drivers;

Driver::
Driver()
{
}

Driver::
~Driver()
{
}

void Driver::
activate()
{
}

uint32_t Driver::
reset()
{
	return 0;
}

void Driver::
deactivate()
{
}

DriverManager::
DriverManager()
:_numDrivers(0)
{
}

DriverManager::
~DriverManager()
{
}

void DriverManager::
addDriver( Driver* drv )
{
	_drivers[_numDrivers] = drv;
	_numDrivers++;
}

void DriverManager::
activateAll()
{
	for( uint32_t i = 0; i < _numDrivers; i++ )
		_drivers[i]->activate();
}


