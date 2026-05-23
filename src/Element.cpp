//std
#include <cmath>
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
static const double w[] = {1, 1};
static const double s[] = {-sqrt(3) / 3, +sqrt(3) / 3};

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
		//point
		p[0] = (1 + s[i]) * (1 + s[j]) / 2 - 1;
		p[1] = (1 - s[i]) * (1 + s[j]) / 2 - 1;
		//return
		return w[i] * w[j] * (1 + s[j]) / 2;
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
	double* Element::position(double* x, const double* p) const
	{
		//data
		double N[6];
		const std::vector<Node>& nodes = m_section->nodes();
		//position
		function(N, p);
		x[0] = x[1] = 0;
		for(uint32_t i = 0; i < 6; i++)
		{
			x[0] += N[i] * nodes[m_nodes[i]].position(0);
			x[1] += N[i] * nodes[m_nodes[i]].position(1);
		}
		//return
		return x;
	}

	//jacobian
	void Element::positions(double* P) const
	{
		//data
		const std::vector<Node>& nodes = m_section->nodes();
		//positions
		for(uint32_t i = 0; i < 6; i++)
		{
			P[0 + 2 * i] = nodes[m_nodes[i]].position(0);
			P[1 + 2 * i] = nodes[m_nodes[i]].position(1);
		}
	}
	double Element::jacobian(const double* p) const
	{
		//data
		math::matrix B(6, 2), P(2, 6);
		//jacobian
		positions(P.data());
		gradient(B.data(), p);
		return (P * B).determinant();
	}
	double Element::spatial_gradient(double* Bx, const double* p) const
	{
		 //data
		math::matrix J(2, 2), B(6, 2), P(2, 6);
		//gradient
		positions(P.data());
		gradient(B.data(), p);
		//gradient
		J = P * B;
		math::matrix(Bx, 6, 2) = B * J.inverse();
		//return
		return J.determinant();
	}

	//assemble
	void Element::assemble_force(double* f) const
	{
		//data
		double d, w, p[2], x[2], N[6], B[12];
		const uint32_t nn = m_section->nodes().size();
		//assemble
		for(uint32_t k = 0; k < 4; k++)
		{
			w = point(p, k);
			function(N, p);
			position(x, p);
			d = spatial_gradient(B, p);
			for(uint32_t i = 0; i < 6; i++)
			{
				const uint32_t di = m_nodes[i];
				f[di + 1 * nn] += w * d * x[1] * N[i];
				f[di + 2 * nn] += w * d * x[0] * N[i];
				f[di + 0 * nn] += w * d * (x[0] * B[i + 0] - x[1] * B[i + 6]);
			}
		}
	}
	void Element::assemble_stiffness(double* K) const
	{
		//data
		double d, w, p[2], B[12];
		const uint32_t nn = m_section->nodes().size();
		//assemble
		for(uint32_t k = 0; k < 4; k++)
		{
			w = point(p, k);
			d = spatial_gradient(B, p);
			for(uint32_t i = 0; i < 6; i++)
			{
				for(uint32_t j = 0; j < 6; j++)
				{
					const uint32_t di = m_nodes[i];
					const uint32_t dj = m_nodes[j];
					K[di + nn * dj] += w * d * (B[di + 0] * B[dj + 0] + B[di + 6] * B[dj + 6]);
				}
			}
		}
	}
}