#ifndef __MYOS__DRIVERS__DRIVER_H
#define __MYOS__DRIVERS__DRIVER_H

#include <common/types.h>

namespace myos
{
	namespace drivers
	{

		class Driver
		{
		public:
			Driver();
			~Driver();
	
			virtual void activate();
			virtual myos::common::uint32_t reset();
			virtual void deactivate();
		};

		class DriverManager
		{
		private:
			Driver* _drivers[256];
			myos::common::uint32_t _numDrivers;

		public:
			DriverManager();
			~DriverManager();
			void addDriver( Driver* );	
			void activateAll();
		};
	}
}

#endif // __MYOS__DRIVERS__DRIVER_H
