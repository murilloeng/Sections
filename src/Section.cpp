//strd
#include <cfloat>

//gmsh
#include <gmsh.h>

//Sections
#include "Sections/inc/Section.hpp"

namespace sections
{
	//constructor
	Section::Section(void) : 
		m_status{false}, m_mesh_size{0}, m_area{0}, m_inertia{0, 0},
		m_shear_area{0, 0, 0}, m_shear_center{0, 0}, m_torsion_constant{0}, m_warping_constant{0}, 
		m_plastic_center{0, 0}, m_elastic_modulus{0, 0}, m_plastic_modulus{0, 0}
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

	//mesh
	bool Section::status(void) const
	{
		return m_status;
	}
	double Section::mesh_size(void) const
	{
		return m_mesh_size;
	}
	double Section::mesh_size(double mesh_size)
	{
		return m_mesh_size = mesh_size;
	}

	const std::vector<Node>& Section::nodes(void) const
	{
		return m_nodes;
	}
	const std::vector<Element>& Section::elements(void) const
	{
		return m_elements;
	}

	//analysis
	void Section::compute(void)
	{
		setup_mesh();
		compute_area();
		compute_center();
		compute_inertia();
		compute_plastic_center();
		compute_elastic_modulus();
		compute_plastic_modulus();
	}

	//print
	void Section::print(void) const
	{
		printf("Area: %+.2e\n", m_area);
		printf("Inertia 2: %+.2e\n", m_inertia[0]);
		printf("Inertia 3: %+.2e\n", m_inertia[1]);
		printf("Elastic modulus 2: %+.2e\n", m_elastic_modulus[0]);
		printf("Elastic modulus 3: %+.2e\n", m_elastic_modulus[1]);
	}

	//mesh
	void Section::setup_mesh(void)
	{
		//initialize
		gmsh::initialize();
		//model
		gmsh::model::add("Section");
		//geometry
		setup_geometry();
		gmsh::model::geo::synchronize();
		//mesh
		gmsh::model::mesh::generate(2);
		gmsh::model::mesh::setOrder(2);
		//recover
		setup_nodes();
		setup_elements();
		//finalize
		gmsh::finalize();
	}
	void Section::setup_nodes(void)
	{
		//mesh
		std::vector<std::size_t> tags;
		std::vector<double> coordinates;
		std::vector<double> parametric_coordinates;
		gmsh::model::mesh::getNodes(tags, coordinates, parametric_coordinates);
		//nodes
		m_nodes.resize(tags.size());
		for(std::size_t i = 0; i < tags.size(); i++)
		{
			for(uint32_t j = 0; j < 2; j++)
			{
				m_nodes[i].m_position[j] = coordinates[3 * i + j];
			}
		}
	}
	void Section::setup_elements(void)
	{
		//mesh
		std::vector<int32_t> types;
		std::vector<std::vector<std::size_t>> tags;
		std::vector<std::vector<std::size_t>> nodes;
		gmsh::model::mesh::getElements(types, tags, nodes);
		//elements
		for(uint32_t i = 0; i < types.size(); i++)
		{
			if(types[i] != 9) continue;
			m_elements.resize(tags[i].size());
			for(uint32_t j = 0; j < tags[i].size(); j++)
			{
				m_elements[j].m_section = this;
				for(uint32_t k = 0; k < 6; k++)
				{
					m_elements[j].m_nodes[k] = nodes[i][6 * j + k] - 1;
				}
			}
		}
	}

	//compute
	void Section::compute_area(void)
	{
		m_area = 0;
		double d, w, p[2];
		for(const Element& element : m_elements)
		{
			for(uint32_t i = 0; i < 4; i++)
			{
				//point
				w = element.point(p, i);
				d = element.jacobian(p);
				//area
				m_area += w * d;
			}
		}
	}
	void Section::compute_center(void)
	{
		double xc[] = {0, 0};
		double d, w, p[2], x[2];
		for(const Element& element : m_elements)
		{
			for(uint32_t i = 0; i < 4; i++)
			{
				//point
				w = element.point(p, i);
				d = element.jacobian(p);
				//moment
				element.position(x, p);
				xc[0] += w * d * x[0] / m_area;
				xc[1] += w * d * x[1] / m_area;
			}
		}
		//apply
		for(Node& node : m_nodes)
		{
			node.m_position[0] -= xc[0];
			node.m_position[1] -= xc[1];
		}
	}
	void Section::compute_inertia(void)
	{
		//inertia
		double d, w, p[2], x[2];
		double inertia[3] = {0, 0, 0};
		for(const Element& element : m_elements)
		{
			for(uint32_t i = 0; i < 4; i++)
			{
				//point
				w = element.point(p, i);
				d = element.jacobian(p);
				//inertia
				element.position(x, p);
				inertia[0] += w * d * x[0] * x[0];
				inertia[1] += w * d * x[1] * x[1];
				inertia[2] += w * d * x[0] * x[1];
			}
		}
		//principal
		const double t = atan(2 * inertia[2] / (inertia[0] - inertia[1]));
		m_inertia[0] = (inertia[0] + inertia[1]) / 2 + cos(t) * (inertia[0] - inertia[1]) / 2 + sin(t) * inertia[2];
		m_inertia[1] = (inertia[0] + inertia[1]) / 2 + cos(t) * (inertia[1] - inertia[0]) / 2 - sin(t) * inertia[2];
		//rotation
		for(Node& node : m_nodes)
		{
			const double x2 = node.m_position[1];
			const double x3 = node.m_position[0];
			node.m_position[0] = cos(t / 2) * x3 + sin(t / 2) * x2;
			node.m_position[1] = cos(t / 2) * x2 - sin(t / 2) * x3;
		}
	}
	void Section::compute_plastic_center(void)
	{
		//data
		double xl[] = {+DBL_MAX, -DBL_MAX, +DBL_MAX, -DBL_MAX};
	}
	void Section::compute_elastic_modulus(void)
	{
		double distance[] = {0, 0};
		for(const Node& node : m_nodes)
		{
			distance[0] = fmax(distance[0], fabs(node.m_position[0]));
			distance[1] = fmax(distance[1], fabs(node.m_position[1]));
		}
		m_elastic_modulus[0] = m_inertia[0] / distance[0];
		m_elastic_modulus[1] = m_inertia[1] / distance[1];
	}
	void Section::compute_plastic_modulus(void)
	{
		return;
	}
}