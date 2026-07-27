#pragma once

//Sections
#include "Sections/inc/Fibers/Fiber.hpp"

namespace sections
{
	namespace fibers
	{
		class Rectangle: public Fiber
		{
		public:
			//constructor
			Rectangle(double, double, double, double);

			//destructor
			~Rectangle(void);

			//data
			double width(void) const;
			double height(void) const;

		private:
			//data
			double m_width;
			double m_height;
		};
	}
}