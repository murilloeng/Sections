#pragma once

//Sections
#include "Sections/inc/Section.hpp"

namespace sections
{
	class RHS : public Section
	{
	public:
		//constructor
		RHS(void);

		//destructor
		~RHS(void);

		//data
		double width(double);
		double width(void) const;

		double height(double);
		double height(void) const;

		double thickness(double);
		double thickness(void) const;

		double radius_inner(double);
		double radius_inner(void) const;

		double radius_outer(double);
		double radius_outer(void) const;

	private:
		//geometry
		void setup_geometry_1(void) const;
		void setup_geometry_2(void) const;
		void setup_geometry_3(void) const;
		void setup_geometry_4(void) const;

		void setup_isolated(void) override;
		void setup_geometry(void) const override;

		//data
		double m_width;
		double m_height;
		double m_thickness;
		double m_radius_inner;
		double m_radius_outer;
	};
}