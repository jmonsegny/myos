#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "types.h"
#include "interrupts.h"
#include "driver.h"
#include "port.h"

class KeyboardEventHandler
{
public:
	KeyboardEventHandler();

	virtual void onKeyDown( int8_t );
	virtual void onKeyUp( int8_t );
};

class KeyboardDriver : public InterruptHandler, public Driver
{
	Port8bit dataport;
	Port8bit commandport;

	KeyboardEventHandler* _handler;
public:
	KeyboardDriver( InterruptManager* manager, KeyboardEventHandler* handler );
	~KeyboardDriver();
	virtual uint32_t HandleInterrupt( uint32_t esp );
	virtual void activate() override;
};

#endif // __KEYBOARD_H
