//std
#include <cmath>

//Sections
#include "Sections/inc/Fibers/Arc.hpp"

namespace sections
{
	namespace fibers
	{
		//constructors
		Arc::Arc(double angle, double start, double radius_1, double radius_2) : m_angle{angle}, m_start{start}, m_radius{radius_1, radius_2}
		{
			//data
			const double a = m_angle;
			const double q = m_start;
			const double r1 = m_radius[0];
			const double r2 = m_radius[1];
			//setup
			m_area = a / 2 * (r2 * r2 - r1 * r1);
			m_position[0] = 2 * (r1 * r1 + r1 * r2 + r2 * r2) / (r1 + r2) * (sin(q + a) - sin(q)) / a / 3;
			m_position[1] = 2 * (r1 * r1 + r1 * r2 + r2 * r2) / (r1 + r2) * (cos(q) - cos(q + a)) / a / 3;
		}

		//destructor
		Arc::~Arc(void)
		{
			return;
		}

		//data
		double Arc::angle(void) const
		{
			return m_angle;
		}
		double Arc::start(void) const
		{
			return m_start;
		}
		double Arc::radius(uint32_t index) const
		{
			return m_radius[index];
		}
	}
}