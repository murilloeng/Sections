//Sections
#include "Sections/inc/Fibers/Fiber.hpp"

namespace sections
{
	namespace fibers
	{
		//constructor
		Fiber::Fiber(void)
		{
			return;
		}
		
		//destructor
		Fiber::~Fiber(void)
		{
			return;
		}

		//data
		double Fiber::area(void) const
		{
			return m_area;
		}

		const double* Fiber::warping(void) const
		{
			return m_warping;
		}
		double Fiber::warping(uint32_t index) const
		{
			return m_warping[index];
		}

		const double* Fiber::position(void) const
		{
			return m_position;
		}
		double Fiber::position(uint32_t index) const
		{
			return m_position[index];
		}

		const double* Fiber::gradient(void) const
		{
			return m_gradient;
		}
		double Fiber::gradient(uint32_t iw, uint32_t iz) const
		{
			return m_gradient[2 * iw + iz];
		}
	}
}