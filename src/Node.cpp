//std
#include <cstdio>

//Sections
#include "Sections/inc/Node.hpp"

namespace sections
{
	//constructor
	Node::Node(void) : m_warping{0, 0, 0}, m_position{0, 0}, m_warping_gradient{0, 0, 0, 0, 0, 0}
	{
		return;
	}
	
	//destructor
	Node::~Node(void)
	{
		return;
	}

	//data
	const double* Node::warping(void) const
	{
		return m_warping;
	}
	double Node::warping(uint32_t index) const
	{
		return m_warping[index];
	}

	const double* Node::position(void) const
	{
		return m_position;
	}
	double Node::position(uint32_t index) const
	{
		return m_position[index];
	}

	const double* Node::warping_gradient(void) const
	{
		return m_warping_gradient;
	}
	double Node::warping_gradient(uint32_t index_warping, uint32_t index_coordinate) const
	{
		return m_warping_gradient[3 * index_warping + index_coordinate];
	}

	//print
	void Node::print(void) const
	{
		//position
		printf("Position: ");
		for(const double& position : m_position) printf("%+.6e ", position);
		//warping
		printf("Warping:");
		for(const double& warping : m_warping) printf("%+.6e ", warping);
		//end line
		printf("\n");
	}
}