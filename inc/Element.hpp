#pragma once

//std
#include <cstdint>

namespace sections
{
	class Section;

	class Element
	{
	public:
		//constructor
		Element(void);

		//Destructor
		~Element(void);

		//data
		uint32_t node(uint32_t) const;
		const uint32_t* node(void) const;

		//print
		void print(void) const;

	private:
		//data
		uint32_t m_nodes[6];

		//friends
		friend class Section;
	};
}