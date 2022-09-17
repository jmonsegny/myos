#ifndef __MYOS__GUI__DESKTOP_H
#define __MYOS__GUI__DESKTOP_H

#include <gui/widget.h>
#include <drivers/mouse.h>

namespace myos
{
	namespace gui 
	{
		class Desktop: public CompositeWidget, public myos::drivers::MouseEventHandler
		{
		protected:
			myos::common::uint32_t _mouseX;
			myos::common::uint32_t _mouseY;
		public:
			Desktop( myos::common::int32_t w, 
			         myos::common::int32_t h, 
			         myos::common::uint8_t r, 
			         myos::common::uint8_t g, 
			         myos::common::uint8_t b );
			~Desktop();

			virtual void draw( myos::common::GraphicsContext* gc ) override;
			virtual void onMouseDown( myos::common::uint8_t button ) override;
            virtual void onMouseUp( myos::common::uint8_t button) override;
            virtual void onMouseMove( myos::common::int32_t x, myos::common::int32_t y) override;	
		};
	}
}

#endif // __MYOS__GUI__DESKTOP_H
