//Sections
#include "Sections/inc/Profile.hpp"

namespace sections
{
	//constructor
	Profile::Profile(void) : m_radius{1.00e-02}, 
		m_web_height{1.00e-01}, m_flange_width{1.00e-01}, m_web_thickness{1.00e-02}, m_flange_thickness{1.00e-02},
		m_fibers_web_height{10}, m_fibers_flange_width{10}, m_fibers_web_thickness{3}, m_fibers_flange_thickness{3}
	{
		return;
	}

	//destructor
	Profile::~Profile(void)
	{
		return;
	}

	//data
	double Profile::radius(void) const
	{
		return m_radius;
	}
	double Profile::radius(double radius)
	{
		return m_radius = radius;
	}

	double Profile::web_height(void) const
	{
		return m_web_height;
	}
	double Profile::web_height(double web_height)
	{
		return m_web_height = web_height;
	}

	double Profile::flange_width(void) const
	{
		return m_flange_width;
	}
	double Profile::flange_width(double flange_width)
	{
		return m_flange_width = flange_width;
	}

	double Profile::web_thickness(void) const
	{
		return m_web_thickness;
	}
	double Profile::web_thickness(double web_thickness)
	{
		return m_web_thickness = web_thickness;
	}

	double Profile::flange_thickness(void) const
	{
		return m_flange_thickness;
	}
	double Profile::flange_thickness(double flange_thickness)
	{
		return m_flange_thickness = flange_thickness;
	}

	uint32_t Profile::fibers_web_height(void) const
	{
		return m_fibers_web_height;
	}
	uint32_t Profile::fibers_web_height(uint32_t fibers_web_height)
	{
		return m_fibers_web_height = fibers_web_height;
	}

	uint32_t Profile::fibers_flange_width(void) const
	{
		return m_fibers_flange_width;
	}
	uint32_t Profile::fibers_flange_width(uint32_t fibers_flange_width)
	{
		return m_fibers_flange_width = fibers_flange_width;
	}

	uint32_t Profile::fibers_web_thickness(void) const
	{
		return m_fibers_web_thickness;
	}
	uint32_t Profile::fibers_web_thickness(uint32_t fibers_web_thickness)
	{
		return m_fibers_web_thickness = fibers_web_thickness;
	}

	uint32_t Profile::fibers_flange_thickness(void) const
	{
		return m_fibers_flange_thickness;
	}
	uint32_t Profile::fibers_flange_thickness(uint32_t fibers_flange_thickness)
	{
		return m_fibers_flange_thickness = fibers_flange_thickness;
	}
}