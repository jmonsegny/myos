#ifndef __MYOS__DRIVERS__MOUSE_H
#define __MYOS__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>
#include <hardwarecommunication/port.h>

namespace myos
{
namespace drivers
{

class MouseEventHandler
{
public:
    MouseEventHandler();

    virtual void onMouseDown( myos::common::uint8_t but );
    virtual void onMouseUp( myos::common::uint8_t but );
    virtual void onMouseMove( myos::common::int32_t xoff, myos::common::int32_t yoff );
    virtual void onActivate();
};

class MouseDriver : public myos::hardwarecommunication::InterruptHandler, public Driver
{
    myos::hardwarecommunication::Port8bit _dataport;
    myos::hardwarecommunication::Port8bit _commandport;

    myos::common::uint8_t _buffer[3];
    myos::common::uint8_t _offset;
    myos::common::uint8_t _buttons;

    MouseEventHandler* _handler;
public:
    MouseDriver( myos::hardwarecommunication::InterruptManager* manager, MouseEventHandler* handler );
    ~MouseDriver();
    virtual myos::common::uint32_t handleInterrupt( myos::common::uint32_t esp );
    virtual void activate() override;
};
}
}

#endif // __MYOS__DRIVERS__MOUSE_H
