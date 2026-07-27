#pragma once

//std
#include <cmath>

//Sections
#include "Sections/inc/Section.hpp"

namespace sections
{
	class Circle : public Section
	{
	public:
		//constructor
		Circle(void);

		//destructor
		~Circle(void);

		//data
		double diameter(double);
		double diameter(void) const;

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
		uint32_t m_fibers_angle;
		uint32_t m_fibers_radius;
	};
}