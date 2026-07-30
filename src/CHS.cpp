//std
#include <cstring>
#include <stdexcept>

//gmsh
#include <gmsh.h>

//Sections
#include "Sections/inc/CHS.hpp"
#include "Sections/inc/Fibers/Arc.hpp"
#include "Sections/inc/Catalog/CHS.hpp"

namespace sections
{
	//constructor
	CHS::CHS(void) : m_diameter{2.13e-02}, m_thickness{2.30e-03}, m_fibers_angle{10}, m_fibers_radius{5}
	{
		return;
	}
	CHS::CHS(const char* label) : m_fibers_angle{10}, m_fibers_radius{5}
	{
		catalog(label);
	}

	//destructor
	CHS::~CHS(void)
	{
		return;
	}

	//data
	void CHS::catalog(const char* label)
	{
		bool test = false;
		const uint32_t units = 1.00e-03;
		for(const catalog::CHS& item : catalog::CHS::m_list)
		{
			if(!test && strcmp(label, item.m_label) == 0)
			{
				test = true;
				m_diameter = units * item.m_diameter;
				m_thickness = units * item.m_thickness;
			}
		}
		if(!test)
		{
			throw std::runtime_error("Error: CHS label not found in catalog!");
		}
	}

	double CHS::diameter(void) const
	{
		return m_diameter;
	}
	double CHS::diameter(double diameter)
	{
		return m_diameter = diameter;
	}

	double CHS::thickness(void) const
	{
		return m_thickness;
	}
	double CHS::thickness(double thickness)
	{
		return m_thickness = thickness;
	}

	uint32_t CHS::fibers_angle(void) const
	{
		return m_fibers_angle;
	}
	uint32_t CHS::fibers_angle(uint32_t fibers_angle)
	{
		return m_fibers_angle = fibers_angle;
	}

	uint32_t CHS::fibers_radius(void) const
	{
		return m_fibers_radius;
	}
	uint32_t CHS::fibers_radius(uint32_t fibers_radius)
	{
		return m_fibers_radius = fibers_radius;
	}

	//geometry
	void CHS::setup_fibers(void)
	{
		//data
		const uint32_t na = m_fibers_angle;
		const uint32_t nr = m_fibers_radius;
		//fibers
		const double t = m_thickness;
		const double a = 2 * M_PI / na;
		const double r = m_diameter / 2;
		for(uint32_t i = 0; i < na; i++)
		{
			for(uint32_t j = 0; j < nr; j++)
			{
				const double q = 2 * M_PI * i / na;
				const double r1 = r - t + t * (j + 0) / nr;
				const double r2 = r - t + t * (j + 1) / nr;
				m_fibers.push_back(new fibers::Arc(a, q, r1, r2));
			}
		}
	}
	void CHS::setup_isolated(void)
	{
		m_isolated = { 0 };
	}
	void CHS::setup_geometry(void) const
	{
		//data
		const double t = m_thickness;
		const double r = m_diameter / 2;
		//points
		gmsh::model::geo::addPoint(0, 0, 0, m_mesh_size, 1);
		gmsh::model::geo::addPoint(r * cos(0 * 2 * M_PI / 3), r * sin(0 * 2 * M_PI / 3), 0, m_mesh_size, 2);
		gmsh::model::geo::addPoint(r * cos(1 * 2 * M_PI / 3), r * sin(1 * 2 * M_PI / 3), 0, m_mesh_size, 3);
		gmsh::model::geo::addPoint(r * cos(2 * 2 * M_PI / 3), r * sin(2 * 2 * M_PI / 3), 0, m_mesh_size, 4);
		gmsh::model::geo::addPoint((r - t) * cos(0 * 2 * M_PI / 3), (r - t) * sin(0 * 2 * M_PI / 3), 0, m_mesh_size, 5);
		gmsh::model::geo::addPoint((r - t) * cos(1 * 2 * M_PI / 3), (r - t) * sin(1 * 2 * M_PI / 3), 0, m_mesh_size, 6);
		gmsh::model::geo::addPoint((r - t) * cos(2 * 2 * M_PI / 3), (r - t) * sin(2 * 2 * M_PI / 3), 0, m_mesh_size, 7);
		//curves
		gmsh::model::geo::addCircleArc(2, 1, 3, 1);
		gmsh::model::geo::addCircleArc(3, 1, 4, 2);
		gmsh::model::geo::addCircleArc(4, 1, 2, 3);
		gmsh::model::geo::addCircleArc(5, 1, 6, 4);
		gmsh::model::geo::addCircleArc(6, 1, 7, 5);
		gmsh::model::geo::addCircleArc(7, 1, 5, 6);
		//loops
		gmsh::model::geo::addCurveLoop({1, 2, 3}, 1);
		gmsh::model::geo::addCurveLoop({4, 5, 6}, 2);
		//surfaces
		gmsh::model::geo::addPlaneSurface({1, -2}, 1);

	}
}