//gmsh
#include <gmsh.h>

//Sections
#include "Sections/inc/Circle.hpp"
#include "Sections/inc/Fibers/Arc.hpp"

namespace sections
{
	//constructor
	Circle::Circle(void) : m_diameter{8.00e-03}, m_fibers_angle{10}, m_fibers_radius{10}
	{
		return;
	}

	//destructor
	Circle::~Circle(void)
	{
		return;
	}

	//data
	double Circle::diameter(void) const
	{
		return m_diameter;
	}
	double Circle::diameter(double diameter)
	{
		return m_diameter = diameter;
	}

	uint32_t Circle::fibers_angle(void) const
	{
		return m_fibers_angle;
	}
	uint32_t Circle::fibers_angle(uint32_t fibers_angle)
	{
		return m_fibers_angle = fibers_angle;
	}

	uint32_t Circle::fibers_radius(void) const
	{
		return m_fibers_radius;
	}
	uint32_t Circle::fibers_radius(uint32_t fibers_radius)
	{
		return m_fibers_radius = fibers_radius;
	}

	//geometry
	void Circle::setup_fibers(void)
	{
		//data
		const uint32_t na = m_fibers_angle;
		const uint32_t nr = m_fibers_radius;
		//fibers
		const double a = 2 * M_PI / na;
		const double r = m_diameter / 2;
		for(uint32_t i = 0; i < na; i++)
		{
			for(uint32_t j = 0; j < nr; j++)
			{
				const double q = 2 * M_PI * i / na;
				const double r1 = r * (j + 0) / m_fibers_radius;
				const double r2 = r * (j + 1) / m_fibers_radius;
				m_fibers.push_back(new fibers::Arc(a, q, r1, r2));
			}
		}
	}
	void Circle::setup_isolated(void)
	{
		m_isolated = { 0 };
	}
	void Circle::setup_geometry(void) const
	{
		//data
		const double r = m_diameter / 2;
		//points
		gmsh::model::geo::addPoint(0, 0, 0, m_mesh_size, 1);
		gmsh::model::geo::addPoint(r * cos(0 * 2 * M_PI / 3), r * sin(0 * 2 * M_PI / 3), 0, m_mesh_size, 2);
		gmsh::model::geo::addPoint(r * cos(1 * 2 * M_PI / 3), r * sin(1 * 2 * M_PI / 3), 0, m_mesh_size, 3);
		gmsh::model::geo::addPoint(r * cos(2 * 2 * M_PI / 3), r * sin(2 * 2 * M_PI / 3), 0, m_mesh_size, 4);
		//curves
		gmsh::model::geo::addCircleArc(2, 1, 3, 1);
		gmsh::model::geo::addCircleArc(3, 1, 4, 2);
		gmsh::model::geo::addCircleArc(4, 1, 2, 3);
		//loops
		gmsh::model::geo::addCurveLoop({1, 2, 3}, 1);
		//surfaces
		gmsh::model::geo::addPlaneSurface({1}, 1);

	}
}