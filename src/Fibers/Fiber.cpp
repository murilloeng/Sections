//Sections
#include "Sections/inc/Fibers/Fiber.hpp"
#include "Sections/inc/Fibers/Rectangle.hpp"

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

		//create
		void Fiber::create_rectangles(std::vector<Fiber*>& fibers, double x2, double x3, double s2, double s3, uint32_t n2, uint32_t n3)
		{
			const double d2 = s2 / n2;
			const double d3 = s3 / n3;
			for(uint32_t i3 = 0; i3 < n3; i3++)
			{
				for(uint32_t i2 = 0; i2 < n2; i2++)
				{
					const double z2 = x2 + i2 * d2 + d2 / 2;
					const double z3 = x3 + i3 * d3 + d3 / 2;
					fibers.push_back(new Rectangle(z2, z3, d2, d3));
				}
			}
		}

		//analysis
		void Fiber::setup(Section* section)
		{
			return;
		}
	}
}