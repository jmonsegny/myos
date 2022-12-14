#ifndef __MYOS__HARDWARECOMMUNICATION__PORT_H
#define __MYOS__HARDWARECOMMUNICATION__PORT_H

#include <common/types.h>

namespace myos
{
namespace hardwarecommunication
{

class Port
{
protected:
    myos::common::uint16_t _portnumber;
    Port( myos::common::uint16_t portnumber );
    ~Port();
};

class Port8bit : public Port
{
public:
    Port8bit( myos::common::uint16_t portnumber );
    ~Port8bit();
    virtual void write( myos::common::uint8_t data );
    virtual myos::common::uint8_t read();
};

class Port8bitSlow : public Port8bit
{
public:
    Port8bitSlow( myos::common::uint16_t portnumber );
    ~Port8bitSlow();
    virtual void write( myos::common::uint8_t data );
};

class Port16bit : public Port
{
public:
    Port16bit( myos::common::uint16_t portnumber );
    ~Port16bit();
    virtual void write( myos::common::uint16_t data );
    virtual myos::common::uint16_t read();
};

class Port32bit : public Port
{
public:
    Port32bit( myos::common::uint16_t portnumber );
    ~Port32bit();
    virtual void write( myos::common::uint32_t data );
    virtual myos::common::uint32_t read();
};
}
}

#endif // __MYOS__HARDWARECOMMUNICATION__PORT_H
