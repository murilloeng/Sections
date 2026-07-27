//Test
#include "Sections/Test/inc/What.hpp"

//constructor
What::What(void) : m_nodes{true}, m_fibers{true}, m_elements{true}
{
	return;
}

//destructor
What::~What(void)
{
	return;
}

//data
bool What::nodes(void) const
{
	return m_nodes;
}
bool What::nodes(bool nodes)
{
	return m_nodes = nodes;
}

bool What::fibers(void) const
{
	return m_fibers;
}
bool What::fibers(bool fibers)
{
	return m_fibers = fibers;
}

bool What::elements(void) const
{
	return m_elements;
}
bool What::elements(bool elements)
{
	return m_elements = elements;
}