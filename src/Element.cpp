//std
#include <cstdio>

//Sections
#include "Sections/inc/Node.hpp"
#include "Sections/inc/Element.hpp"
#include "Sections/inc/Section.hpp"

//Math
#include "Math/inc/linear/matrix.hpp"

//static
static const double A[] = {
	+0, +1, +1, +1, +2, +1,
	+0, +1, +0, +1, +0, +0,
	+0, +0, +1, +0, +0, +1,
	+0, -2, -2, -2, -2, +0,
	+2, +2, +2, +0, +2, +0,
	+0, -2, -2, +0, -2, -2
};

namespace sections
{
	//constructor
	Element::Element(void) : m_nodes{0, 0, 0, 0, 0, 0}
	{
		return;
	}
	
	//destructor
	Element::~Element(void)
	{
		return;
	}

	//data
	const uint32_t* Element::node(void) const
	{
		return m_nodes;
	}
	uint32_t Element::node(uint32_t index) const
	{
		return m_nodes[index];
	}

	//print
	void Element::print(void) const
	{
		for(const uint32_t& node : m_nodes)
		{
			printf("%d ", node);
		}
		printf("\n");
	}

	//interpolation
	double Element::point(double* p, uint32_t index) const
	{
		//data
		const uint32_t i = index / 2;
		const uint32_t j = index % 2;
		//quadrature
		const double e = m_quadrature.point(i);
		const double n = m_quadrature.point(j);
		const double wi = m_quadrature.weight(i);
		const double wj = m_quadrature.weight(j);
		//point
		p[0] = (1 + e) * (1 + n) / 2 - 1;
		p[1] = (1 - e) * (1 + n) / 2 - 1;
		//return
		return wi * wj * (1 + n) / 2;
	}
	double* Element::function(double* N, const double* p) const
	{
		//data
		const double v[] = {
			1, p[0], p[1], p[0] * p[0], p[0] * p[1], p[1] * p[1]
		};
		//shape
		for(uint32_t i = 0; i < 6; i++)
		{
			N[i] = 0;
			for(uint32_t j = 0; j < 6; j++)
			{
				N[i] += A[6 * i + j] / 2 * v[j];
			}
		}
		return N;
	}
	double* Element::gradient(double* B, const double* p) const
	{
		//data
		const double dv1[] = {
			0, 1, 0, 2 * p[0], p[1], 0
		};
		const double dv2[] = {
			0, 0, 1, 0, p[0], 2 * p[1]
		};
		//gradient
		for(uint32_t i = 0; i < 6; i++)
		{
			B[6 * 0 + i] = 0;
			B[6 * 1 + i] = 0;
			for(uint32_t j = 0; j < 6; j++)
			{
				B[i + 6 * 0] += A[6 * i + j] / 2 * dv1[j];
				B[i + 6 * 1] += A[6 * i + j] / 2 * dv2[j];
			}
		}
		//return
		return B;
	}

	//jacobian
	void Element::positions(double* P) const
	{
		//data
		const std::vector<Node>& nodes = m_section->nodes();
		//positions
		for(uint32_t i = 0; i < 6; i++)
		{
			P[2 * i + 0] = nodes[m_nodes[i]].position(0);
			P[2 * i + 1] = nodes[m_nodes[i]].position(1);
		}
	}
	double Element::jacobian(double* J, const double* p) const
	{
		//data
		double B[12], P[12];
		//jacobian
		positions(P);
		gradient(B, p);
		math::matrix(J, 2, 2) = math::matrix(P, 2, 6) * math::matrix(B, 6, 2);
		//return
		return math::matrix(J, 2, 2).determinant();
	}

	//static data
	math::quadrature::Quadrature Element::m_quadrature(2);
}