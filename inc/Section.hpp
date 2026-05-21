#pragma once

//std
#include <vector>

//Sections
#include "Sections/inc/Node.hpp"
#include "Sections/inc/Element.hpp"

namespace sections
{
	class Section
	{
	public:
		//constructors
		Section(void);

		//destructor
		virtual ~Section(void);

		//data
		double area(void) const;
		double inertia(uint32_t) const;
		
		double shear_area(uint32_t) const;
		double shear_center(uint32_t) const;
		
		double torsion_constant(void) const;
		double warping_constant(void) const;
		
		double elastic_modulus(uint32_t) const;
		double plastic_modulus(uint32_t) const;

		//mesh
		bool status(void) const;
		double mesh_size(double);
		double mesh_size(void) const;

		const std::vector<Node>& nodes(void) const;
		const std::vector<Element>& elements(void) const;

		//compute
		void compute(void);

	protected:
		//setup
		void setup_nodes(void);
		void setup_elements(void);

		//geometry
		virtual void setup_geometry(void) const = 0;

		//data
		bool m_status;
		double m_mesh_size;

		double m_area;
		double m_inertia[3];
		double m_shear_area[3];
		double m_shear_center[2];
		double m_torsion_constant;
		double m_warping_constant;
		double m_elastic_modulus[2];
		double m_plastic_modulus[2];

		std::vector<Node> m_nodes;
		std::vector<Element> m_elements;
	};
}