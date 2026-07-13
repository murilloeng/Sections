#pragma once

//std
#include <vector>

//Sections
#include "Sections/inc/Section.hpp"

namespace sections
{
	class Generic : public Section
	{
	public:
		//constructors
		Generic(void);

		//destructor
		virtual ~Generic(void);

		//data
		double area(double);
		double inertia(uint32_t, double);

		double shear_area(uint32_t, double);
		double shear_center(uint32_t, double);

		double torsion_constant(double);
		double warping_constant(double);

		double plastic_center(uint32_t, double);
		double elastic_modulus(uint32_t, double);
		double plastic_modulus(uint32_t, double);

		using Section::area, Section::inertia;
		using Section::shear_area, Section::shear_center;
		using Section::torsion_constant, Section::warping_constant;
		using Section::plastic_center, Section::elastic_modulus, Section::plastic_modulus;

	private:
		//compute
		void compute(void);
	};
}