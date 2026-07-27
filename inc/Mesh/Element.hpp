#pragma once

//std
#include <cstdint>

namespace sections
{
	class Section;

	class Element
	{
	public:
		//constructor
		Element(void);

		//Destructor
		~Element(void);

		//data
		uint32_t node(uint32_t) const;
		const uint32_t* node(void) const;

		//print
		void print(void) const;

	private:
		//assemble
		void assemble_force(void) const;
		void assemble_stiffness(void) const;

		//warping
		void warping_center(double*) const;
		void warping_properties(double*) const;

		//compute
		void compute_area(double&) const;
		void compute_center(double*) const;
		void compute_inertia(double*) const;
		void compute_plastic_modulus(double*) const;

		//plastic center
		void plastic_center_2(double&, double) const;
		void plastic_center_3(double&, double) const;

		//jacobian
		void positions(double*) const;
		double jacobian(const double*) const;
		double jacobian(double*, const double*) const;

		//interpolation
		double point(double*, uint32_t) const;
		double* function(double*, const double*) const;
		double* gradient(double*, const double*) const;

		double* warping(double*, const double*) const;
		double* position(double*, const double*) const;

		//data
		Section* m_section;
		uint32_t m_nodes[6];

		//friends
		friend class Section;
	};
}