#ifndef __DRIVER_H
#define __DRIVER_H

#include "types.h"

class Driver
{
public:
	Driver();
	~Driver();
	
	virtual void activate();
	virtual uint32_t reset();
	virtual void deactivate();
};

class DriverManager
{
private:
	Driver* _drivers[256];
	uint32_t _numDrivers;

public:
	DriverManager();
	~DriverManager();
	void addDriver( Driver* );	
	void activateAll();
};

#endif // __DRIVER_H
