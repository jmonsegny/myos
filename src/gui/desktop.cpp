#include <gui/desktop.h>
using namespace myos::gui;
using namespace myos::common;

Desktop::
Desktop( int32_t w, int32_t h, uint8_t r, uint8_t g, uint8_t b )
:CompositeWidget( 0, 0, 0, w, h, r, g, b ),
 MouseEventHandler()
{
	_mouseX = w/2;
	_mouseY = h/2;
}

Desktop::
~Desktop()
{
}

void Desktop::
draw( GraphicsContext* gc )
{
	CompositeWidget::draw( gc );
	for( int i = 0; i < 4; i++ ) {
		gc->putPixel( _mouseX-i, _mouseY,   0xFF, 0xFF, 0xFF);
		gc->putPixel( _mouseX+i, _mouseY,   0xFF, 0xFF, 0xFF);
		gc->putPixel( _mouseX,   _mouseY-i, 0xFF, 0xFF, 0xFF);
		gc->putPixel( _mouseX,   _mouseY+i, 0xFF, 0xFF, 0xFF);
	}
}

void Desktop::
onMouseDown( uint8_t button )
{
	CompositeWidget::onMouseDown( _mouseX, _mouseY, button );
}

void Desktop::
onMouseUp( uint8_t button)
{
	CompositeWidget::onMouseUp( _mouseX, _mouseY, button );
}

void Desktop::
onMouseMove( int32_t x, int32_t y )
{
	//x /= 4;
	//y /= 4;

	int32_t newMouseX = _mouseX + x;
	if( newMouseX < 0 )
		newMouseX = 0;
	if( newMouseX >= _w )
        newMouseX = _w - 1 ;
	
	int32_t newMouseY = _mouseY + y; 
	if( newMouseY < 0 )
        newMouseY = 0;
    if( newMouseY >= _h )
        newMouseY = _h - 1 ;

	CompositeWidget::onMouseMove( _mouseX, _mouseY, newMouseX, newMouseY );
	_mouseX = newMouseX;
	_mouseY = newMouseY;
}

