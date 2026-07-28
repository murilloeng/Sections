//std
#include <cstdio>

//Sections
#include "Sections/inc/Section.hpp"
#include "Sections/inc/Mesh/Element.hpp"
#include "Sections/inc/Fibers/Fiber.hpp"
#include "Sections/inc/Fibers/Rectangle.hpp"

namespace sections
{
	namespace fibers
	{
		//constructor
		Fiber::Fiber(void) : 
			m_area{0}, m_warping{0, 0, 0}, m_position{0, 0}, m_gradient{0, 0, 0, 0, 0, 0}, m_residual_stress{nullptr}
		{
			return;
		}
		
		//destructor
		Fiber::~Fiber(void)
		{
			delete[] m_residual_stress;
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
			return m_gradient[3 * iz + iw];
		}

		materials::Point& Fiber::material_point(void)
		{
			return m_material_point;
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
			for(const sections::Element& element : section->elements())
			{
				if(element.inside(m_position))
				{
					element.fiber_setup(*this);
					break;
				}
			}
		}
	}
}