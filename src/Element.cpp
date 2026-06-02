//std
#include <cmath>
#include <cstdio>

//Sections
#include "Sections/inc/Node.hpp"
#include "Sections/inc/Element.hpp"
#include "Sections/inc/Section.hpp"

//Math
#include "Math/inc/Linear/Matrix.hpp"

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

	//assemble
	void Element::assemble_force(void) const
	{
		//data
		double* f = m_section->m_f;
		const uint32_t nn = m_section->m_nodes.size();
		double d, w, p[2], x[2], J[4], N[6], Bn[12], Bs[12];
		//assemble
		for(uint32_t k = 0; k < 4; k++)
		{
			//point
			w = point(p, k);
			d = jacobian(J, p);
			//gradient
			function(N, p);
			position(x, N);
			gradient(Bn, p);
			for(uint32_t i = 0; i < 6; i++)
			{
				Bs[i + 0] = (J[3] * Bn[i + 0] - J[1] * Bn[i + 6]) / d;
				Bs[i + 6] = (J[0] * Bn[i + 6] - J[2] * Bn[i + 0]) / d;
			}
			//force
			for(uint32_t i = 0; i < 6; i++)
			{
				const uint32_t di = m_nodes[i];
				f[di + 1 * nn] += w * d * x[0] * N[i];
				f[di + 2 * nn] += w * d * x[1] * N[i];
				f[di + 0 * nn] += w * d * (x[1] * Bs[i + 0] - x[0] * Bs[i + 6]);
			}
		}
	}
	void Element::assemble_stiffness(void) const
	{
		//data
		double* K = m_section->m_K;
		double d, w, p[2], J[4], Bn[12], Bs[12];
		const uint32_t nn = m_section->m_nodes.size();
		//assemble
		for(uint32_t k = 0; k < 4; k++)
		{
			//point
			w = point(p, k);
			d = jacobian(J, p);
			//gradient
			gradient(Bn, p);
			for(uint32_t i = 0; i < 6; i++)
			{
				Bs[i + 0] = (J[3] * Bn[i + 0] - J[1] * Bn[i + 6]) / d;
				Bs[i + 6] = (J[0] * Bn[i + 6] - J[2] * Bn[i + 0]) / d;
			}
			//stiffness
			for(uint32_t i = 0; i < 6; i++)
			{
				for(uint32_t j = 0; j < 6; j++)
				{
					const uint32_t di = m_nodes[i];
					const uint32_t dj = m_nodes[j];
					K[di + nn * dj] += w * d * Bs[i + 0] * Bs[j + 0];
					K[di + nn * dj] += w * d * Bs[i + 6] * Bs[j + 6];
				}
			}
		}
	}

	//warping
	void Element::warping_center(double* Q) const
	{
		double d, w, p[2], N[6], u[3];
		for(uint32_t k = 0; k < 4; k++)
		{
			//point
			w = point(p, k);
			d = jacobian(p);
			//function
			function(N, p);
			//warping
			warping(u, N);
			Q[0] += w * d * u[0];
			Q[1] += w * d * u[1];
			Q[2] += w * d * u[2];
		}
	}
	void Element::warping_properties(double* H) const
	{
		double d, w;
		double p[2], N[6], x[2], u[3];
		for(uint32_t k = 0; k < 4; k++)
		{
			//point
			w = point(p, k);
			d = jacobian(p);
			//function
			function(N, p);
			//warping
			warping(u, N);
			position(x, N);
			H[0] -= w * d * x[1] * u[0];
			H[1] += w * d * x[0] * u[0];
			H[2] += w * d * x[0] * u[1];
			H[3] += w * d * x[1] * u[2];
			H[4] += w * d * x[0] * u[2];
			H[5] += w * d * u[0] * u[0];
		}
	}

	//compute
	void Element::compute_area(double& A) const
	{
		double d, w, p[2];
		for(uint32_t k = 0; k < 4; k++)
		{
			//point
			w = point(p, k);
			d = jacobian(p);
			//area
			A += w * d;
		}
	}
	void Element::compute_center(double* Q) const
	{
		double d, w, p[2], N[6], x[2];
		for(uint32_t k = 0; k < 4; k++)
		{
			//point
			w = point(p, k);
			d = jacobian(p);
			//position
			function(N, p);
			position(x, N);
			//center
			Q[0] += w * d * x[0];
			Q[1] += w * d * x[1];
		}
	}
	void Element::compute_inertia(double* I) const
	{
		double d, w, p[2], N[6], x[2];
		for(uint32_t k = 0; k < 4; k++)
		{
			//point
			w = point(p, k);
			d = jacobian(p);
			//position
			function(N, p);
			position(x, N);
			//inertia
			I[0] += w * d * x[1] * x[1];
			I[1] += w * d * x[0] * x[0];
			I[2] += w * d * x[0] * x[1];
		}
	}
	void Element::compute_plastic_modulus(double* Z) const
	{
		double d, w, p[2], N[6], x[2];
		for(uint32_t k = 0; k < 4; k++)
		{
			//point
			w = point(p, k);
			d = jacobian(p);
			//position
			function(N, p);
			position(x, N);
			//inertia
			Z[0] += w * d * fabs(x[1] - m_section->m_plastic_center[1]);
			Z[1] += w * d * fabs(x[0] - m_section->m_plastic_center[0]);
		}
	}

	//plastic center
	void Element::plastic_center_2(double& A, double x2) const
	{
		double d, w, p[2], N[6], x[2];
		for(uint32_t k = 0; k < 4; k++)
		{
			//point
			w = point(p, k);
			d = jacobian(p);
			//area
			function(N, p);
			position(x, N);
			A += w * d * (x[0] < x2 ? -1 : +1);
		}
	}
	void Element::plastic_center_3(double& A, double x3) const
	{
		double d, w, p[2], N[6], x[2];
		for(uint32_t k = 0; k < 4; k++)
		{
			//point
			w = point(p, k);
			d = jacobian(p);
			//area
			function(N, p);
			position(x, N);
			A += w * d * (x[1] < x3 ? -1 : +1);
		}
	}

	//jacobian
	void Element::positions(double* P) const
	{
		for(uint32_t i = 0; i < 6; i++)
		{
			P[0 + 2 * i] = m_section->m_nodes[m_nodes[i]].m_position[0];
			P[1 + 2 * i] = m_section->m_nodes[m_nodes[i]].m_position[1];
		}
	}
	double Element::jacobian(const double* p) const
	{
		//data
		math::Matrix B(6, 2), P(2, 6);
		//jacobian
		positions(P.data());
		gradient(B.data(), p);
		return (P * B).determinant();
	}
	double Element::jacobian(double* J, const double* p) const
	{
		 //data
		math::Matrix B(6, 2), P(2, 6);
		//jacobian
		positions(P.data());
		gradient(B.data(), p);
		math::Matrix(J, 2, 2) = P * B;
		//return
		return math::Matrix(J, 2, 2).determinant();
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

	double* Element::warping(double* u, const double* N) const
	{
		//warping
		u[0] = u[1] = u[2] = 0;
		for(uint32_t i = 0; i < 6; i++)
		{
			u[0] += N[i] * m_section->m_nodes[m_nodes[i]].m_warping[0];
			u[1] += N[i] * m_section->m_nodes[m_nodes[i]].m_warping[1];
			u[2] += N[i] * m_section->m_nodes[m_nodes[i]].m_warping[2];
		}
		//return
		return u;
	}
	double* Element::position(double* x, const double* N) const
	{
		//position
		x[0] = x[1] = 0;
		for(uint32_t i = 0; i < 6; i++)
		{
			x[0] += N[i] * m_section->m_nodes[m_nodes[i]].m_position[0];
			x[1] += N[i] * m_section->m_nodes[m_nodes[i]].m_position[1];
		}
		//return
		return x;
	}
}