//std
#include <cstdio>

//Sections
#include "Sections/inc/Element.hpp"

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
}