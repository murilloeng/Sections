//gmsh
#include <gmsh.h>

//Sections
#include "Sections/inc/Section.hpp"

namespace sections
{
	//constructor
	Section::Section(void) : 
		m_status{false}, m_mesh_size{0},
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

	//setup
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
				for(uint32_t k = 0; k < 6; k++)
				{
					m_elements[j].m_nodes[k] = nodes[i][6 * j + k] - 1;
				}
			}
		}
	}
}