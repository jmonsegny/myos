#include <gui/widget.h>
using namespace myos::drivers;
using namespace myos::gui;
using namespace myos::common;

Widget::
Widget( Widget* parent, int32_t x, int32_t y, int32_t w, int32_t h, uint8_t r, uint8_t g, uint8_t b )
:KeyboardEventHandler(),_parent(parent), _x(x), _y(y), _w(w), _h(h), _r(r), _g(g), _b(b),_focussable(true)
{
}

Widget::
~Widget()
{
}

void Widget::
getFocus( Widget* widget )
{
	if( _parent != 0 )
		_parent->getFocus( widget );
}

void Widget::
modelToScreen( int32_t& x, int32_t& y )
{
	if( _parent != 0 )
		_parent->modelToScreen( x, y );
	x += _x;
	y += _y;
}

void Widget::
draw( GraphicsContext* gc )
{
	int x = 0;
	int y = 0;
	modelToScreen( x, y );	
	gc->fillRectangle( x, y, _w, _h, _r, _g, _b );
}

void Widget::
onMouseDown( int32_t x, int32_t y, uint8_t button )
{
	if( _focussable )
        getFocus( this );
}

void Widget::
onMouseUp( int32_t x, int32_t y, uint8_t button )
{
}

void Widget::
onMouseMove( int32_t oldx, int32_t oldy, int32_t newx, int32_t newy )
{
}

/*void Widget::
onKeyDown( char* str )
{
}

void Widget::
onKeyUp( char* str )
{
}*/

bool Widget::
containsCoordinate( myos::common::int32_t x, myos::common::int32_t y )
{
	return ( x >= _x && x < _x + _w && y >= _y && y < _y + _h );
}

// ----------------------------------------------

CompositeWidget::
CompositeWidget( Widget* parent, int32_t x, int32_t y, int32_t w, int32_t h, uint8_t r, uint8_t g, uint8_t b )
:Widget(parent, x, y, w, h, r, g, b),_focussedChild(0),_numChildren(0)
{
}

CompositeWidget::
~CompositeWidget()
{
}

void CompositeWidget::
getFocus( Widget* widget )
{
	_focussedChild = widget;
    if( _parent != 0 )
        _parent->getFocus( this );
}

bool CompositeWidget::
addChild( Widget* child )
{
	if( _numChildren >= 100 )
		return false;
	_children[_numChildren++] = child;
	return true;
}

void CompositeWidget::
draw( GraphicsContext* gc )
{
	Widget::draw( gc );
	for( int i = _numChildren-1; i >= 0; i-- ) {
		_children[i]->draw( gc );
	}
}

void CompositeWidget::
onMouseDown( int32_t x, int32_t y, uint8_t button )
{
	for( int i = 0; i < _numChildren; i++ ) {
		if( _children[i]->containsCoordinate( x - _x, y - _y ) ) {
	        _children[i]->onMouseDown( x - _x, y - _y, button );
			break;
		}
    }
}

void CompositeWidget::
onMouseUp( int32_t x, int32_t y, uint8_t button )
{
	for( int i = 0; i < _numChildren; i++ ) {
        if( _children[i]->containsCoordinate( x - _x, y - _y ) ) {
            _children[i]->onMouseUp( x - _x, y - _y, button );
            break;
        }
    }
}

void CompositeWidget::
onMouseMove( int32_t oldx, int32_t oldy, int32_t newx, int32_t newy )
{
	int firstchild = -1;
	for( int i = 0; i < _numChildren; i++ ) {
        if( _children[i]->containsCoordinate( oldx - _x, oldy - _y ) ) {
            _children[i]->onMouseMove( oldx - _x, oldy - _y, newx - _x, newy - _y );
			firstchild = i;
            break;
        }
    }

	for( int i = 0; i < _numChildren; i++ ) {
        if( _children[i]->containsCoordinate( newx - _x, newy - _y ) ) {
			if( firstchild != i )
	            _children[i]->onMouseMove( oldx - _x, oldy - _y, newx - _x, newy - _y );
            break;
        }
    }

}

void CompositeWidget::
onKeyDown( char c )
{
	if( _focussedChild != 0 )
		_focussedChild->onKeyDown( c );
}

void CompositeWidget::
onKeyUp( char c )
{
	if( _focussedChild != 0 )
        _focussedChild->onKeyUp( c );
}


