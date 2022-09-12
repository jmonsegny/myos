#ifndef __MOUSE_H
#define __MOUSE_H

#include "types.h"
#include "interrupts.h"
#include "driver.h"
#include "port.h"

class MouseEventHandler
{
public:
    MouseEventHandler();

    virtual void onMouseDown( uint8_t but );
    virtual void onMouseUp( uint8_t but );
	virtual void onMouseMove( int8_t xoff, int8_t yoff );
	virtual void onActivate();
};

class MouseDriver : public InterruptHandler, public Driver
{
	Port8bit dataport;
	Port8bit commandport;

	uint8_t buffer[3];
	uint8_t offset;
	uint8_t buttons;

	MouseEventHandler* _handler;
public:
	MouseDriver( InterruptManager* manager, MouseEventHandler* handler );
	~MouseDriver();
	virtual uint32_t HandleInterrupt( uint32_t esp );
	virtual void activate() override;
};

#endif // __MOUSE_H
