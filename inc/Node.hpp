#pragma once

//std
#include <cstdint>

namespace sections
{
	class Node
	{
	public:
		//constructor
		Node(void);

		//destructor
		~Node(void);

		//data
		double warping(uint32_t) const;
		const double* warping(void) const;

		double position(uint32_t) const;
		const double* position(void) const;

		const double* warping_gradient(void) const;
		double warping_gradient(uint32_t, uint32_t) const;
	
	private:
		//data
		double m_warping[3];
		double m_position[2];
		double m_warping_gradient[6];
	};
}