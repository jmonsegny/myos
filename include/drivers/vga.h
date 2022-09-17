#ifndef __MYOS__DRIVERS__VGA_H
#define __MYOS__DRIVERS__VGA_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
//#include <hardwarecommunication/interrupts.h>

namespace myos
{
	namespace drivers
	{
		class VideoGraphicsArray
		{
			myos::hardwarecommunication::Port8bit _miscPort;
            myos::hardwarecommunication::Port8bit _crtcIndexPort;
            myos::hardwarecommunication::Port8bit _crtcDataPort;
            myos::hardwarecommunication::Port8bit _sequencerIndexPort;
            myos::hardwarecommunication::Port8bit _sequencerDataPort;
            myos::hardwarecommunication::Port8bit _graphicsControllerIndexPort;
            myos::hardwarecommunication::Port8bit _graphicsControllerDataPort;
            myos::hardwarecommunication::Port8bit _attributeControllerIndexPort;
            myos::hardwarecommunication::Port8bit _attributeControllerReadPort;
            myos::hardwarecommunication::Port8bit _attributeControllerWritePort;
            myos::hardwarecommunication::Port8bit _attributeControllerResetPort;

			myos::common::uint8_t* _frameBuffer;

			void writeRegisters( myos::common::uint8_t* registers );
			myos::common::uint8_t* getFrameBufferSegment();
	
//			virtual void putPixel( myos::common::uint32_t x,
//                                   myos::common::uint32_t y,
//                                   myos::common::uint8_t colorIndex );
			virtual myos::common::uint8_t getColorIndex( myos::common::uint8_t r,
                                                         myos::common::uint8_t g,
                                                         myos::common::uint8_t b );
			
		public:
			VideoGraphicsArray();
			~VideoGraphicsArray();

			virtual bool supportsMode( myos::common::uint32_t width, 
                                       myos::common::uint32_t height, 
                                       myos::common::uint32_t colordepth );

			virtual bool setMode( myos::common::uint32_t width, 
                                  myos::common::uint32_t height, 
                                  myos::common::uint32_t colordepth );

			virtual void putPixel( myos::common::uint32_t x, 
			                       myos::common::uint32_t y, 
			                       myos::common::uint8_t r,
								   myos::common::uint8_t g,
								   myos::common::uint8_t b );
		
			virtual void putPixel( myos::common::uint32_t x,
                                   myos::common::uint32_t y,
                                   myos::common::uint8_t colorIndex );	
			virtual void fillRectangle( myos::common::uint32_t x,
                                        myos::common::uint32_t y,
                                        myos::common::uint32_t w,
                                        myos::common::uint32_t h,
                                        myos::common::uint8_t r,
                                        myos::common::uint8_t g,
                                        myos::common::uint8_t b );
		};		 
	}
}

#endif // __MYOS__DRIVERS__VGA_H
