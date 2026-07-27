//Sections
#include "Sections/inc/Fibers/Rectangle.hpp"

namespace sections
{
	namespace fibers
	{
		//constructor
		Rectangle::Rectangle(double x2, double x3, double w, double h) : m_width{w}, m_height{h}
		{
			m_position[0] = x2;
			m_position[1] = x3;
			m_area = m_width * m_height;
		}
		
		//destructor
		Rectangle::~Rectangle(void)
		{
			return;
		}

		//data
		double Rectangle::width(void) const
		{
			return m_width;
		}
		double Rectangle::height(void) const
		{
			return m_height;
		}
	}
}