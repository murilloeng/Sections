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

		double plastic_center(uint32_t) const;
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

		//print
		void print(void) const;

	protected:
		//setup
		void setup_mesh(void);
		void setup_nodes(void);
		void setup_warping(void);
		void setup_elements(void);

		//compute
		void compute_area(void);
		void compute_center(void);
		void compute_inertia(void);
		void compute_warping(void);
		void compute_properties(void);
		void compute_plastic_center(void);
		void compute_elastic_modulus(void);
		void compute_plastic_modulus(void);

		//warping
		void warping_fix(void);
		void warping_center(void);
		void warping_functions(void);

		//geometry
		virtual void setup_isolated(void);
		virtual void setup_geometry(void) const;

		//plastic center
		static double plastic_center_function_2(double, const void**);
		static double plastic_center_function_3(double, const void**);

		//data
		bool m_status;
		double m_mesh_size;

		double m_area;
		double m_inertia[2];
		double m_shear_area[3];
		double m_shear_center[2];
		double m_torsion_constant;
		double m_warping_constant;
		double m_plastic_center[2];
		double m_elastic_modulus[2];
		double m_plastic_modulus[2];

		double *m_u, *m_f, *m_K;
		std::vector<Node> m_nodes;
		std::vector<Element> m_elements;
		std::vector<uint32_t> m_isolated;

		//friends
		friend class Element;
	};
}