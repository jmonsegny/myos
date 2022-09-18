#ifndef __MYOS__GUI__WIDGET_H
#define __MYOS__GUI__WIDGET_H

#include <common/types.h>
#include <drivers/keyboard.h>
#include <common/graphicscontext.h>

namespace myos
{
namespace gui
{
class Widget : public myos::drivers::KeyboardEventHandler
{
protected:
    Widget* _parent;
    myos::common::int32_t _x;
    myos::common::int32_t _y;
    myos::common::int32_t _w;
    myos::common::int32_t _h;

    myos::common::uint8_t _r;
    myos::common::uint8_t _g;
    myos::common::uint8_t _b;

    bool _focussable;
public:
    Widget( Widget* parent,
            myos::common::int32_t x,
            myos::common::int32_t y,
            myos::common::int32_t w,
            myos::common::int32_t h,
            myos::common::uint8_t r,
            myos::common::uint8_t g,
            myos::common::uint8_t b );
    ~Widget();
    virtual void getFocus( Widget* widget );
    virtual void modelToScreen( myos::common::int32_t& x, myos::common::int32_t& y );
    virtual void draw( myos::common::GraphicsContext* gc );
    virtual void onMouseDown( myos::common::int32_t x,
                              myos::common::int32_t y,
                              myos::common::uint8_t button );
    virtual void onMouseUp( myos::common::int32_t x,
                            myos::common::int32_t y,
                            myos::common::uint8_t button );
    virtual void onMouseMove( myos::common::int32_t oldx, myos::common::int32_t oldy,
                              myos::common::int32_t newx, myos::common::int32_t newy );
//            virtual void onKeyDown( char* str /*myos::common::int32_t x, myos::common::int32_t y*/ );
//            virtual void onKeyUp( char* str /*myos::common::int32_t x, myos::common::int32_t y*/ );

    virtual bool containsCoordinate( myos::common::int32_t x, myos::common::int32_t y );
};

class CompositeWidget : public Widget
{
private:
    Widget* _children[100];
    int _numChildren;
    Widget* _focussedChild;
public:
    CompositeWidget( Widget* parent,
                     myos::common::int32_t x,
                     myos::common::int32_t y,
                     myos::common::int32_t w,
                     myos::common::int32_t h,
                     myos::common::uint8_t r,
                     myos::common::uint8_t g,
                     myos::common::uint8_t b );
    ~CompositeWidget();
    virtual void getFocus( Widget* widget ) override;
    virtual bool addChild( Widget* child );
    virtual void draw( myos::common::GraphicsContext* gc ) override;
    virtual void onMouseDown( myos::common::int32_t x,
                              myos::common::int32_t y,
                              myos::common::uint8_t button ) override;
    virtual void onMouseUp( myos::common::int32_t x,
                            myos::common::int32_t y,
                            myos::common::uint8_t button) override;
    virtual void onMouseMove( myos::common::int32_t oldx, myos::common::int32_t oldy,
                              myos::common::int32_t newx, myos::common::int32_t newy ) override;
    virtual void onKeyDown( char c /*myos::common::int32_t x, myos::common::int32_t y*/ ) override;
    virtual void onKeyUp( char c /*myos::common::int32_t x, myos::common::int32_t y*/ ) override;

};
}
}

#endif // __MYOS__GUI__WIDGET_H
