//Sections
#include "Sections/inc/Section.hpp"

namespace sections
{
	//constructor
	Section::Section(void) : 
		m_area{0}, m_inertia{0, 0, 0},
		m_shear_area{0, 0, 0}, m_shear_center{0, 0},
		m_torsion_constant{0}, m_warping_constant{0}, 
		m_elastic_modulus{0, 0}, m_plastic_modulus{0, 0}
	{
		return;
	}

	//destructor
	Section::~Section(void)
	{
		return;
	}

	//data
	double Section::area(void) const
	{
		return m_area;
	}
	double Section::inertia(uint32_t index) const
	{
		return m_inertia[index];
	}

	double Section::shear_area(uint32_t index) const
	{
		return m_shear_area[index];
	}
	double Section::shear_center(uint32_t index) const
	{
		return m_shear_center[index];
	}

	double Section::torsion_constant(void) const
	{
		return m_torsion_constant;
	}
	double Section::warping_constant(void) const
	{
		return m_warping_constant;
	}
	
	double Section::elastic_modulus(uint32_t index) const
	{
		return m_elastic_modulus[index];
	}
	double Section::plastic_modulus(uint32_t index) const
	{
		return m_plastic_modulus[index];
	}
}