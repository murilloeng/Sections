//std
#include <cfloat>
#include <cstring>
#include <stdexcept>

//gmsh
#include <gmsh.h>

//SuiteSparse
#include "suitesparse/umfpack.h"

//Sections
#include "Sections/inc/Section.hpp"

//Math
#include "Math/inc/Linear/Vector.hpp"
#include "Math/inc/Linear/Sparse.hpp"
#include "Math/inc/Solvers/Bisection.hpp"

namespace sections
{
	//constructor
	Section::Section(void) : 
		m_status{false}, m_mesh_size{0}, 
		m_area{0}, m_inertia{0, 0}, m_shear_area{0, 0, 0}, m_shear_center{0, 0}, 
		m_torsion_constant{0}, m_warping_constant{0}, m_plastic_center{0, 0}, m_elastic_modulus{0, 0}, m_plastic_modulus{0, 0},
		m_rows_map{nullptr}, m_cols_map{nullptr}, m_rows_triplet{nullptr}, m_cols_triplet{nullptr}, m_u{nullptr}, m_f{nullptr}, m_K{nullptr}
	{
		return;
	}

	//destructor
	Section::~Section(void)
	{
		delete[] m_u;
		delete[] m_f;
		delete[] m_K;
		delete[] m_rows_map;
		delete[] m_cols_map;
		delete[] m_rows_triplet;
		delete[] m_cols_triplet;
		for(const fibers::Fiber* fiber : m_fibers) delete fiber;
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

	double Section::plastic_center(uint32_t index) const
	{
		return m_plastic_center[index];
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

	const Node& Section::node(uint32_t index) const
	{
		return m_nodes[index];
	}
	const Element& Section::element(uint32_t index) const
	{
		return m_elements[index];
	}
	const fibers::Fiber* Section::fiber(uint32_t index) const
	{
		return m_fibers[index];
	}

	const std::vector<Node>& Section::nodes(void) const
	{
		return m_nodes;
	}
	const std::vector<Element>& Section::elements(void) const
	{
		return m_elements;
	}
	const std::vector<fibers::Fiber*>& Section::fibers(void) const
	{
		return m_fibers;
	}

	//analysis
	void Section::compute(void)
	{
		setup_mesh();
		compute_area();
		compute_center();
		compute_inertia();
		compute_warping();
		compute_properties();
		compute_plastic_center();
		compute_elastic_modulus();
		compute_plastic_modulus();
		for(fibers::Fiber* fiber : m_fibers) fiber->setup(this);
	}

	//print
	void Section::print(void) const
	{
		printf("Area: %+.2e\n", m_area);
		printf("Inertia 2: %+.2e\n", m_inertia[0]);
		printf("Inertia 3: %+.2e\n", m_inertia[1]);
		printf("Shear Area 2: %+.2e\n", m_shear_area[0]);
		printf("Shear Area 3: %+.2e\n", m_shear_area[1]);
		printf("Shear Center 2: %+.2e\n", m_shear_center[0]);
		printf("Shear Center 3: %+.2e\n", m_shear_center[1]);
		printf("Torsion Constant: %+.2e\n", m_torsion_constant);
		printf("Warping Constant: %+.2e\n", m_warping_constant);
		printf("Plastic Center 2: %+.2e\n", m_plastic_center[0]);
		printf("Plastic Center 3: %+.2e\n", m_plastic_center[1]);
		printf("Elastic Modulus 2: %+.2e\n", m_elastic_modulus[0]);
		printf("Elastic Modulus 3: %+.2e\n", m_elastic_modulus[1]);
		printf("Plastic Modulus 2: %+.2e\n", m_plastic_modulus[0]);
		printf("Plastic Modulus 3: %+.2e\n", m_plastic_modulus[1]);
	}

	//setup
	void Section::setup_mesh(void)
	{
		//initialize
		gmsh::initialize();
		gmsh::option::setNumber("General.Verbosity", 0);
		//model
		gmsh::model::add("Section");
		//geometry
		setup_fibers();
		setup_geometry();
		setup_isolated();
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
			m_nodes[i].m_position[0] = +coordinates[3 * i + 1];
			m_nodes[i].m_position[1] = -coordinates[3 * i + 0];
		}
	}
	void Section::setup_warping(void)
	{
		//data
		const uint32_t nn = m_nodes.size();
		const uint32_t ne = m_elements.size();
		const uint32_t ni = m_isolated.size();
		//setup
		delete[] m_u;
		delete[] m_f;
		delete[] m_K;
		delete[] m_rows_map;
		delete[] m_cols_map;
		delete[] m_rows_triplet;
		delete[] m_cols_triplet;
		m_u = new double[3 * nn];
		m_f = new double[3 * nn];
		m_cols_map = new int32_t[nn + 1];
		m_rows_map = new int32_t[36 * ne + ni];
		m_rows_triplet = new int32_t[36 * ne + ni];
		m_cols_triplet = new int32_t[36 * ne + ni];
		for(uint32_t i = 0; i < ne; i++)
		{
			for(uint32_t j = 0; j < 6; j++)
			{
				for(uint32_t k = 0; k < 6; k++)
				{
					m_rows_triplet[36 * i + 6 * j + k] = m_elements[i].m_nodes[j];
					m_cols_triplet[36 * i + 6 * j + k] = m_elements[i].m_nodes[k];
				}
			}
		}
		for(uint32_t i = 0; i < ni; i++)
		{
			m_rows_triplet[36 * ne + i] = m_isolated[i];
			m_cols_triplet[36 * ne + i] = m_isolated[i];
		}
		umfpack_di_triplet_to_col(nn, nn, 36 * ne + ni, m_rows_triplet, m_cols_triplet, nullptr, m_cols_map, m_rows_map, nullptr, nullptr);
		//setup
		m_K = new double[m_cols_map[nn]];
		memset(m_f, 0, 3 * nn * sizeof(double));
		memset(m_K, 0, m_cols_map[nn] * sizeof(double));
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
		for(const Element& element : m_elements)
		{
			element.compute_area(m_area);
		}
	}
	void Section::compute_center(void)
	{
		//center
		double Q[] = {0, 0};
		for(const Element& element : m_elements)
		{
			element.compute_center(Q);
		}
		//apply
		for(Node& node : m_nodes)
		{
			node.m_position[0] -= Q[0] / m_area;
			node.m_position[1] -= Q[1] / m_area;
		}
	}
	void Section::compute_inertia(void)
	{
		//inertia
		double I[] = {0, 0, 0};
		for(const Element& element : m_elements)
		{
			element.compute_inertia(I);
		}
		//rotation
		const double t = atan(2 * I[2] / (I[0] - I[1]));
		m_inertia[0] = (I[0] + I[1]) / 2 + cos(t) * (I[0] - I[1]) / 2 + sin(t) * I[2];
		m_inertia[1] = (I[0] + I[1]) / 2 + cos(t) * (I[1] - I[0]) / 2 - sin(t) * I[2];
		//apply
		for(Node& node : m_nodes)
		{
			const double x2 = node.m_position[0];
			const double x3 = node.m_position[1];
			node.m_position[0] = cos(t / 2) * x2 + sin(t / 2) * x3;
			node.m_position[1] = cos(t / 2) * x3 - sin(t / 2) * x2;
		}
	}
	void Section::compute_warping(void)
	{
		//data
		const uint32_t nn = m_nodes.size();
		//assemble
		setup_warping();
		for(const Element& element : m_elements)
		{
			element.assemble_force();
			element.assemble_stiffness();
		}
		//solve
		warping_fix();
		if(!math::Sparse(m_K, m_rows_map, m_cols_map, nn, nn).solve(m_u, m_f, 3))
		{
			throw std::runtime_error("Error: Unable to solve warping problem!");
		}
		//apply
		for(uint32_t i = 0; i < nn; i++)
		{
			for(uint32_t j = 0; j < 3; j++)
			{
				m_nodes[i].m_warping[j] = m_u[i + nn * j];
			}
		}
		warping_center();
	}
	void Section::compute_gradients(void)
	{
		//setup
		double Ae;
		const double pv[] = {
			-1, -1, +1, -1, -1, +1, 
			+0, -1, +0, +0, -1, +0
		};
		math::Matrix J(2, 2), B(6, 2), ue(3, 6);
		std::vector<double> An(m_nodes.size(), 0);
		for(Node& node : m_nodes) memset(node.m_gradient, 0, 6 * sizeof(double));
		//assemble
		for(const Element& element : m_elements)
		{
			//area
			Ae = 0;
			element.compute_area(Ae);
			//warping
			for(uint32_t i = 0; i < 6; i++)
			{
				for(uint32_t j = 0; j < 3; j++)
				{
					ue(j, i) = m_nodes[element.m_nodes[i]].m_warping[j];
				}
			}
			//gradient
			for(uint32_t i = 0; i < 6; i++)
			{
				An[element.m_nodes[i]] += Ae;
				element.jacobian(J.data(), pv + 2 * i);
				element.gradient(B.data(), pv + 2 * i);
				math::Matrix(m_nodes[element.m_nodes[i]].m_gradient, 3, 2) += Ae * ue * B * J.inverse();
			}
		}
		//update
		for(uint32_t i = 0; i < m_nodes.size(); i++)
		{
			if(An[i] != 0)
			{
				for(double& gradient : m_nodes[i].m_gradient) gradient /= An[i];
			}
		}
	}
	void Section::compute_properties(void)
	{
		//data
		double& c2 = m_shear_center[0];
		double& c3 = m_shear_center[1];
		const double I2 = m_inertia[0];
		const double I3 = m_inertia[1];
		//compute
		double H[] = {0, 0, 0, 0, 0, 0};
		for(const Element& element : m_elements)
		{
			element.warping_properties(H);
		}
		//properties
		m_shear_center[0] = H[0] / I2;
		m_shear_center[1] = H[1] / I3;
		m_warping_constant = H[5] - c2 * c2 * I2 - c3 * c3 * I3;
		m_shear_area[0] = +I3 * I3 * H[3] / (H[2] * H[3] - H[4] * H[4]);
		m_shear_area[1] = +I2 * I2 * H[2] / (H[2] * H[3] - H[4] * H[4]);
		m_shear_area[2] = -I2 * I3 * H[4] / (H[2] * H[3] - H[4] * H[4]);
		m_torsion_constant = I2 + I3 - math::Vector(m_f, m_nodes.size()).inner(m_u);
		//functions
		warping_functions();
	}
	void Section::compute_plastic_center(void)
	{
		//data
		double x2_min = +DBL_MAX;
		double x2_max = -DBL_MAX;
		double x3_min = +DBL_MAX;
		double x3_max = -DBL_MAX;
		const void* args[] = { this };
		math::solvers::Bisection solver_2, solver_3;
		//bounds
		for(const Node& node : m_nodes)
		{
			x2_min = fmin(x2_min, node.m_position[0]);
			x2_max = fmax(x2_max, node.m_position[0]);
			x3_min = fmin(x3_min, node.m_position[1]);
			x3_max = fmax(x3_max, node.m_position[1]);
		}
		//setup
		solver_2.m_x1 = x2_min;
		solver_2.m_x2 = x2_max;
		solver_3.m_x1 = x3_min;
		solver_3.m_x2 = x3_max;
		solver_2.m_tolerance = 1.00e-5 * m_area;
		solver_3.m_tolerance = 1.00e-5 * m_area;
		solver_2.m_system_2 = Section::plastic_center_function_2;
		solver_3.m_system_2 = Section::plastic_center_function_3;
		//solve
		solver_2.solve(args);
		solver_3.solve(args);
		m_plastic_center[0] = solver_2.m_xs;
		m_plastic_center[1] = solver_3.m_xs;
	}
	void Section::compute_elastic_modulus(void)
	{
		double distance[] = {0, 0};
		for(const Node& node : m_nodes)
		{
			distance[0] = fmax(distance[0], fabs(node.m_position[0]));
			distance[1] = fmax(distance[1], fabs(node.m_position[1]));
		}
		m_elastic_modulus[0] = m_inertia[0] / distance[1];
		m_elastic_modulus[1] = m_inertia[1] / distance[0];
	}
	void Section::compute_plastic_modulus(void)
	{
		m_plastic_modulus[0] = 0;
		m_plastic_modulus[1] = 0;
		for(const Element& element : m_elements)
		{
			element.compute_plastic_modulus(m_plastic_modulus);
		}
	}

	//warping
	void Section::warping_fix(void)
	{
		//data
		const uint32_t nn = m_nodes.size();
		math::Sparse Ks(m_K, m_rows_map, m_cols_map, nn, nn);
		//diagonal
		double Kd = 0;
		for(uint32_t i = 0; i < nn; i++)
		{
			Kd += Ks(i, i);
		}
		//stiffness
		Ks(0, 0) += Kd;
		for(uint32_t i : m_isolated)
		{
			Ks(i, i) += Kd;
		}
	}
	void Section::warping_center(void)
	{
		//compute
		double Q[] = {0, 0, 0};
		for(const Element& element : m_elements)
		{
			element.warping_center(Q);
		}
		//adjust
		for(Node& node : m_nodes)
		{
			node.m_warping[0] -= Q[0] / m_area;
			node.m_warping[1] -= Q[1] / m_area;
			node.m_warping[2] -= Q[2] / m_area;
		}
	}
	void Section::warping_functions(void)
	{
		//data
		const double I2 = m_inertia[0];
		const double I3 = m_inertia[1];
		const double A2 = m_shear_area[0];
		const double A3 = m_shear_area[1];
		const double Am = m_shear_area[2];
		const double c2 = m_shear_center[0];
		const double c3 = m_shear_center[1];
		//warping
		for(Node& node : m_nodes)
		{
			const double wt = node.m_warping[0];
			const double n2 = node.m_warping[1];
			const double n3 = node.m_warping[2];
			const double x2 = node.m_position[0];
			const double x3 = node.m_position[1];
			node.m_warping[0] = wt + c2 * x3 - c3 * x2;
			node.m_warping[1] = A2 / I3 * n2 + Am / I2 * n3;
			node.m_warping[2] = Am / I3 * n2 + A3 / I2 * n3;
		}
		//gradients
		compute_gradients();
	}

	//geometry
	void Section::setup_fibers(void)
	{
		return;
	}
	void Section::setup_isolated(void)
	{
		return;
	}
	void Section::setup_geometry(void) const
	{
		return;
	}

	//plastic center
	double Section::plastic_center_function_2(double x2, const void** args)
	{
		//data
		double A = 0;
		const Section* section = (const Section*) args[0];
		//area
		for(const Element& element : section->m_elements)
		{
			element.plastic_center_2(A, x2);
		}
		//return
		return A;
	}
	double Section::plastic_center_function_3(double x3, const void** args)
	{
		//data
		double A = 0;
		const Section* section = (const Section*) args[0];
		//area
		for(const Element& element : section->m_elements)
		{
			element.plastic_center_3(A, x3);
		}
		//return
		return A;
	}
}