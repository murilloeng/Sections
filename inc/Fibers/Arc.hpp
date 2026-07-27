#pragma once

//std
#include <cstdint>

//Sections
#include "Sections/inc/Fibers/Fiber.hpp"

namespace sections
{
	namespace fibers
	{
		class Arc : public Fiber
		{
		public:
			//constructors
			Arc(double, double, double, double);

			//destructor
			~Arc(void);

			//data
			double angle(void) const;
			double start(void) const;
			double radius(uint32_t) const;

		private:
			//data
			double m_angle;
			double m_start;
			double m_radius[2];
		};
	}
}