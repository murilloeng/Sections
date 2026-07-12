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
		
		double elastic_modulus(uint32_t, double);
		double plastic_modulus(uint32_t, double);

		//compute
		void compute(void);
	};
}