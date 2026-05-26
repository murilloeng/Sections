//gmsh
#include <gmsh.h>

//Sections
#include "Sections/inc/CHS.hpp"

namespace sections
{
	//constructor
	CHS::CHS(void) : m_diameter{2.13e-02}, m_thickness{2.30e-03}
	{
		return;
	}
	
	//destructor
	CHS::~CHS(void)
	{
		return;
	}

	//data
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

	//geometry
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