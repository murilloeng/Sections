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
		CHS(const char*);

		//destructor
		~CHS(void);

		//data
		void catalog(const char*);

		double diameter(double);
		double diameter(void) const;

		double thickness(double);
		double thickness(void) const;

		uint32_t fibers_angle(uint32_t);
		uint32_t fibers_angle(void) const;

		uint32_t fibers_radius(uint32_t);
		uint32_t fibers_radius(void) const;

	private:
		//geometry
		void setup_fibers(void) override;
		void setup_isolated(void) override;
		void setup_geometry(void) const override;

		//data
		double m_diameter;
		double m_thickness;
		uint32_t m_fibers_angle;
		uint32_t m_fibers_radius;
	};
}