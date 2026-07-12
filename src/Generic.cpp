//std
#include <cfloat>
#include <cstring>
#include <stdexcept>

//gmsh
#include <gmsh.h>

//Sections
#include "Sections/inc/Generic.hpp"

namespace sections
{
	//constructor
	Generic::Generic(void)
	{
		return;
	}

	//destructor
	Generic::~Generic(void)
	{
		return;
	}

	//data
	double Generic::area(double area)
	{
		return m_area = area;
	}
	double Generic::inertia(uint32_t index, double inertia)
	{
		return m_inertia[index] = inertia;
	}

	double Generic::shear_area(uint32_t index, double shear_area)
	{
		return m_shear_area[index] = shear_area;
	}
	double Generic::shear_center(uint32_t index, double shear_center)
	{
		return m_shear_center[index] = shear_center;
	}

	double Generic::torsion_constant(double torsion_constant)
	{
		return m_torsion_constant = torsion_constant;
	}
	double Generic::warping_constant(double warping_constant)
	{
		return m_warping_constant = warping_constant;
	}

	double Generic::plastic_center(uint32_t index, double plastic_center)
	{
		return m_plastic_center[index] = plastic_center;
	}
	double Generic::elastic_modulus(uint32_t index, double elastic_modulus)
	{
		return m_elastic_modulus[index] = elastic_modulus;
	}
	double Generic::plastic_modulus(uint32_t index, double plastic_modulus)
	{
		return m_plastic_modulus[index] = plastic_modulus;
	}

	void Generic::compute(void)
	{
		return;
	}
}