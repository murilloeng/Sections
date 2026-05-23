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
		//interpolation
		double point(double*, uint32_t) const;
		double* function(double*, const double*) const;
		double* gradient(double*, const double*) const;
		double* position(double*, const double*) const;

		//jacobian
		void positions(double*) const;
		double jacobian(const double*) const;
		double spatial_gradient(double*, const double*) const;

		//assemble
		void assemble_force(double*) const;
		void assemble_stiffness(double*) const;

		//data
		Section* m_section;
		uint32_t m_nodes[6];

		//friends
		friend class Section;
	};
}