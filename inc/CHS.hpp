#pragma once

//Sections
#include "Sections/inc/Section.hpp"

namespace sections
{
	class CHS : public Section
	{
	public:
		//constructor
		CHS(void);

		//destructor
		~CHS(void);

		//data
		double diameter(double);
		double diameter(void) const;

		double thickness(double);
		double thickness(void) const;

	private:
		//geometry
		void setup_isolated(void) override;
		void setup_geometry(void) const override;

		//data
		double m_diameter;
		double m_thickness;
	};
}