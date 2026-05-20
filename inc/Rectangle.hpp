#pragma once

//Sections
#include "Sections/inc/Section.hpp"

namespace sections
{
	class Rectangle : public Section
	{
	public:
		//constructor
		Rectangle(void);

		//destructor
		~Rectangle(void);

		//data
		double width(double);
		double width(void) const;

		double height(double);
		double height(void) const;

	private:
		//geometry
		void setup_geometry(void) const override;

		//data
		double m_width;
		double m_height;
	};
}