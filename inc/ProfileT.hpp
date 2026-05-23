#pragma once

//Sections
#include "Sections/inc/Section.hpp"

namespace sections
{
	class ProfileT : public Section
	{
	public:
		//constructor
		ProfileT(void);

		//destructor
		~ProfileT(void);

		//data
		double radius(double);
		double radius(void) const;

		double web_height(double);
		double web_height(void) const;

		double flange_width(double);
		double flange_width(void) const;

		double web_thickness(double);
		double web_thickness(void) const;

		double flange_thickness(double);
		double flange_thickness(void) const;

	private:
		//geometry
		void setup_geometry_1(void) const;
		void setup_geometry_2(void) const;
		void setup_geometry(void) const override;

		//data
		double m_radius;
		double m_web_height;
		double m_flange_width;
		double m_web_thickness;
		double m_flange_thickness;
	};
}