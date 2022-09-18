#include <gui/window.h>
using namespace myos::common;
using namespace myos::gui;

Window::
Window( Widget* parent, int32_t x, int32_t y, int32_t w, int32_t h, uint8_t r, uint8_t g, uint8_t b )
    :CompositeWidget( parent, x, y, w, h, r, g, b ), _dragging( false )
{
}

Window::
~Window()
{
}

void Window::
onMouseDown( int32_t x, int32_t y, uint8_t button )
{
    _dragging = (button == 1);
    CompositeWidget::onMouseDown( x, y, button );
}

void Window::
onMouseUp( int32_t x, int32_t y, uint8_t button)
{
    _dragging = false;
    CompositeWidget::onMouseUp( x, y, button );
}

void Window::
onMouseMove( int32_t oldx, int32_t oldy, int32_t newx, int32_t newy )
{
    if( _dragging ) {
        _x += newx - oldx;
        _y += newy - oldy;
    }
    CompositeWidget::onMouseMove( oldx, oldy, newx, newy );
}

