#ifndef __MYOS__DRIVERS__KEYBOARD_H
#define __MYOS__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>
#include <hardwarecommunication/port.h>

namespace myos
{
namespace drivers
{

class KeyboardEventHandler
{
public:
    KeyboardEventHandler();

    virtual void onKeyDown( myos::common::int8_t );
    virtual void onKeyUp( myos::common::int8_t );
};

class KeyboardDriver : public myos::hardwarecommunication::InterruptHandler, public Driver
{
    myos::hardwarecommunication::Port8bit _dataport;
    myos::hardwarecommunication::Port8bit _commandport;

    KeyboardEventHandler* _handler;
public:
    KeyboardDriver( myos::hardwarecommunication::InterruptManager* manager, KeyboardEventHandler* handler );
    ~KeyboardDriver();
    virtual myos::common::uint32_t handleInterrupt( myos::common::uint32_t esp );
    virtual void activate() override;
};
}
}

#endif // __MYOS__DRIVERS__KEYBOARD_H
