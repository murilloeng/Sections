//gmsh
#include <gmsh.h>

//Sections
#include "Sections/inc/RHS.hpp"

namespace sections
{
	//constructor
	RHS::RHS(void) : m_width{3.00e-02}, m_height{5.00e-02}, m_thickness{2.60e-03}, m_radius_inner{2.60e-03}, m_radius_outer{3.90e-03}
	{
		return;
	}
	
	//destructor
	RHS::~RHS(void)
	{
		return;
	}

	//data
	double RHS::width(void) const
	{
		return m_width;
	}
	double RHS::width(double width)
	{
		return m_width = width;
	}

	double RHS::height(void) const
	{
		return m_height;
	}
	double RHS::height(double height)
	{
		return m_height = height;
	}

	double RHS::thickness(void) const
	{
		return m_thickness;
	}
	double RHS::thickness(double thickness)
	{
		return m_thickness = thickness;
	}

	double RHS::radius_inner(void) const
	{
		return m_radius_inner;
	}
	double RHS::radius_inner(double radius_inner)
	{
		return m_radius_inner = radius_inner;
	}

	double RHS::radius_outer(void) const
	{
		return m_radius_outer;
	}
	double RHS::radius_outer(double radius_outer)
	{
		return m_radius_outer = radius_outer;
	}

	//geometry
	void RHS::setup_geometry_1(void) const
	{
		//data
		const double w = m_width;
		const double h = m_height;
		const double t = m_thickness;
		//points
		gmsh::model::geo::addPoint(-w / 2, -h / 2, 0, m_mesh_size, 1);
		gmsh::model::geo::addPoint(+w / 2, -h / 2, 0, m_mesh_size, 2);
		gmsh::model::geo::addPoint(+w / 2, +h / 2, 0, m_mesh_size, 3);
		gmsh::model::geo::addPoint(-w / 2, +h / 2, 0, m_mesh_size, 4);
		gmsh::model::geo::addPoint(-w / 2 + t, -h / 2 + t, 0, m_mesh_size, 5);
		gmsh::model::geo::addPoint(+w / 2 - t, -h / 2 + t, 0, m_mesh_size, 6);
		gmsh::model::geo::addPoint(+w / 2 - t, +h / 2 - t, 0, m_mesh_size, 7);
		gmsh::model::geo::addPoint(-w / 2 + t, +h / 2 - t, 0, m_mesh_size, 8);
		//curves
		gmsh::model::geo::addLine(1, 2, 1);
		gmsh::model::geo::addLine(2, 3, 2);
		gmsh::model::geo::addLine(3, 4, 3);
		gmsh::model::geo::addLine(4, 1, 4);
		gmsh::model::geo::addLine(5, 6, 5);
		gmsh::model::geo::addLine(6, 7, 6);
		gmsh::model::geo::addLine(7, 8, 7);
		gmsh::model::geo::addLine(8, 5, 8);
		//loops
		gmsh::model::geo::addCurveLoop({1, 2, 3, 4}, 1);
		gmsh::model::geo::addCurveLoop({5, 6, 7, 8}, 2);
		//surfaces
		gmsh::model::geo::addPlaneSurface({1, 2}, 1);
	}
	void RHS::setup_geometry_2(void) const
	{
		//data
		const double w = m_width;
		const double h = m_height;
		const double t = m_thickness;
		const double r = m_radius_outer;
		//points
		gmsh::model::geo::addPoint(-w / 2, -h / 2 + r, 0, m_mesh_size, 1);
		gmsh::model::geo::addPoint(-w / 2 + r, -h / 2, 0, m_mesh_size, 2);
		gmsh::model::geo::addPoint(+w / 2 - r, -h / 2, 0, m_mesh_size, 3);
		gmsh::model::geo::addPoint(+w / 2, -h / 2 + r, 0, m_mesh_size, 4);
		gmsh::model::geo::addPoint(+w / 2, +h / 2 - r, 0, m_mesh_size, 5);
		gmsh::model::geo::addPoint(+w / 2 - r, +h / 2, 0, m_mesh_size, 6);
		gmsh::model::geo::addPoint(-w / 2 + r, +h / 2, 0, m_mesh_size, 7);
		gmsh::model::geo::addPoint(-w / 2, +h / 2 - r, 0, m_mesh_size, 8);
		gmsh::model::geo::addPoint(-w / 2 + r, -h / 2 + r, 0, m_mesh_size,  9);
		gmsh::model::geo::addPoint(+w / 2 - r, -h / 2 + r, 0, m_mesh_size, 10);
		gmsh::model::geo::addPoint(+w / 2 - r, +h / 2 - r, 0, m_mesh_size, 11);
		gmsh::model::geo::addPoint(-w / 2 + r, +h / 2 - r, 0, m_mesh_size, 12);
		gmsh::model::geo::addPoint(-w / 2 + t, -h / 2 + t, 0, m_mesh_size, 13);
		gmsh::model::geo::addPoint(+w / 2 - t, -h / 2 + t, 0, m_mesh_size, 14);
		gmsh::model::geo::addPoint(+w / 2 - t, +h / 2 - t, 0, m_mesh_size, 15);
		gmsh::model::geo::addPoint(-w / 2 + t, +h / 2 - t, 0, m_mesh_size, 16);
		//curves
		gmsh::model::geo::addLine(2, 3, 1);
		gmsh::model::geo::addLine(4, 5, 2);
		gmsh::model::geo::addLine(6, 7, 3);
		gmsh::model::geo::addLine(8, 1, 4);
		gmsh::model::geo::addLine(13, 14, 5);
		gmsh::model::geo::addLine(14, 15, 6);
		gmsh::model::geo::addLine(15, 16, 7);
		gmsh::model::geo::addLine(16, 13, 8);
		gmsh::model::geo::addCircleArc(1,  9, 2,  9);
		gmsh::model::geo::addCircleArc(3, 10, 4, 10);
		gmsh::model::geo::addCircleArc(5, 11, 6, 11);
		gmsh::model::geo::addCircleArc(7, 12, 8, 12);
		//loops
		gmsh::model::geo::addCurveLoop({5, 6, 7, 8}, 2);
		gmsh::model::geo::addCurveLoop({9, 1, 10, 2, 11, 3, 12, 4}, 1);
		//surfaces
		gmsh::model::geo::addPlaneSurface({1, 2}, 1);
	}
	void RHS::setup_geometry_3(void) const
	{
		//data
		const double w = m_width;
		const double h = m_height;
		const double t = m_thickness;
		const double r = m_radius_inner;
		//points
		gmsh::model::geo::addPoint(-w / 2, -h / 2, 0, m_mesh_size, 1);
		gmsh::model::geo::addPoint(+w / 2, -h / 2, 0, m_mesh_size, 2);
		gmsh::model::geo::addPoint(+w / 2, +h / 2, 0, m_mesh_size, 3);
		gmsh::model::geo::addPoint(-w / 2, +h / 2, 0, m_mesh_size, 4);
		gmsh::model::geo::addPoint(-w / 2 + t, -h / 2 + t + r, 0, m_mesh_size,  5);
		gmsh::model::geo::addPoint(-w / 2 + t + r, -h / 2 + t, 0, m_mesh_size,  6);
		gmsh::model::geo::addPoint(+w / 2 - t - r, -h / 2 + t, 0, m_mesh_size,  7);
		gmsh::model::geo::addPoint(+w / 2 - t, -h / 2 + t + r, 0, m_mesh_size,  8);
		gmsh::model::geo::addPoint(+w / 2 - t, +h / 2 - t - r, 0, m_mesh_size,  9);
		gmsh::model::geo::addPoint(+w / 2 - t - r, +h / 2 - t, 0, m_mesh_size, 10);
		gmsh::model::geo::addPoint(-w / 2 + t + r, +h / 2 - t, 0, m_mesh_size, 11);
		gmsh::model::geo::addPoint(-w / 2 + t, +h / 2 - t - r, 0, m_mesh_size, 12);
		gmsh::model::geo::addPoint(-w / 2 + t + r, -h / 2 + t + r, 0, m_mesh_size, 13);
		gmsh::model::geo::addPoint(+w / 2 - t - r, -h / 2 + t + r, 0, m_mesh_size, 14);
		gmsh::model::geo::addPoint(+w / 2 - t - r, +h / 2 - t - r, 0, m_mesh_size, 15);
		gmsh::model::geo::addPoint(-w / 2 + t + r, +h / 2 - t - r, 0, m_mesh_size, 16);
		//curves
		gmsh::model::geo::addLine( 1,  2, 1);
		gmsh::model::geo::addLine( 2,  3, 2);
		gmsh::model::geo::addLine( 3,  4, 3);
		gmsh::model::geo::addLine( 4,  1, 4);
		gmsh::model::geo::addLine( 6,  7, 5);
		gmsh::model::geo::addLine( 8,  9, 6);
		gmsh::model::geo::addLine(10, 11, 7);
		gmsh::model::geo::addLine(12,  5, 8);
		gmsh::model::geo::addCircleArc( 5, 13,  6,  9);
		gmsh::model::geo::addCircleArc( 7, 14,  8, 10);
		gmsh::model::geo::addCircleArc( 9, 15, 10, 11);
		gmsh::model::geo::addCircleArc(11, 16, 12, 12);
		//loops
		gmsh::model::geo::addCurveLoop({1, 2, 3, 4}, 1);
		gmsh::model::geo::addCurveLoop({9, 5, 10, 6, 11, 7, 12, 8}, 2);
		//surfaces
		gmsh::model::geo::addPlaneSurface({1, 2}, 1);
	}
	void RHS::setup_geometry_4(void) const
	{
		//data
		const double w = m_width;
		const double h = m_height;
		const double t = m_thickness;
		const double r1 = m_radius_inner;
		const double r2 = m_radius_outer;
		//points
		gmsh::model::geo::addPoint(-w / 2, -h / 2 + r2, 0, m_mesh_size, 1);
		gmsh::model::geo::addPoint(-w / 2 + r2, -h / 2, 0, m_mesh_size, 2);
		gmsh::model::geo::addPoint(+w / 2 - r2, -h / 2, 0, m_mesh_size, 3);
		gmsh::model::geo::addPoint(+w / 2, -h / 2 + r2, 0, m_mesh_size, 4);
		gmsh::model::geo::addPoint(+w / 2, +h / 2 - r2, 0, m_mesh_size, 5);
		gmsh::model::geo::addPoint(+w / 2 - r2, +h / 2, 0, m_mesh_size, 6);
		gmsh::model::geo::addPoint(-w / 2 + r2, +h / 2, 0, m_mesh_size, 7);
		gmsh::model::geo::addPoint(-w / 2, +h / 2 - r2, 0, m_mesh_size, 8);
		gmsh::model::geo::addPoint(-w / 2 + r2, -h / 2 + r2, 0, m_mesh_size,  9);
		gmsh::model::geo::addPoint(+w / 2 - r2, -h / 2 + r2, 0, m_mesh_size, 10);
		gmsh::model::geo::addPoint(+w / 2 - r2, +h / 2 - r2, 0, m_mesh_size, 11);
		gmsh::model::geo::addPoint(-w / 2 + r2, +h / 2 - r2, 0, m_mesh_size, 12);
		gmsh::model::geo::addPoint(-w / 2 + t, -h / 2 + t + r1, 0, m_mesh_size, 13);
		gmsh::model::geo::addPoint(-w / 2 + t + r1, -h / 2 + t, 0, m_mesh_size, 14);
		gmsh::model::geo::addPoint(+w / 2 - t - r1, -h / 2 + t, 0, m_mesh_size, 15);
		gmsh::model::geo::addPoint(+w / 2 - t, -h / 2 + t + r1, 0, m_mesh_size, 16);
		gmsh::model::geo::addPoint(+w / 2 - t, +h / 2 - t - r1, 0, m_mesh_size, 17);
		gmsh::model::geo::addPoint(+w / 2 - t - r1, +h / 2 - t, 0, m_mesh_size, 18);
		gmsh::model::geo::addPoint(-w / 2 + t + r1, +h / 2 - t, 0, m_mesh_size, 19);
		gmsh::model::geo::addPoint(-w / 2 + t, +h / 2 - t - r1, 0, m_mesh_size, 20);
		gmsh::model::geo::addPoint(-w / 2 + t + r1, -h / 2 + t + r1, 0, m_mesh_size, 21);
		gmsh::model::geo::addPoint(+w / 2 - t - r1, -h / 2 + t + r1, 0, m_mesh_size, 22);
		gmsh::model::geo::addPoint(+w / 2 - t - r1, +h / 2 - t - r1, 0, m_mesh_size, 23);
		gmsh::model::geo::addPoint(-w / 2 + t + r1, +h / 2 - t - r1, 0, m_mesh_size, 24);
		//curves
		gmsh::model::geo::addLine( 2,  3, 1);
		gmsh::model::geo::addLine( 4,  5, 2);
		gmsh::model::geo::addLine( 6,  7, 3);
		gmsh::model::geo::addLine( 8,  1, 4);
		gmsh::model::geo::addLine(14, 15, 5);
		gmsh::model::geo::addLine(16, 17, 6);
		gmsh::model::geo::addLine(18, 19, 7);
		gmsh::model::geo::addLine(20, 13, 8);
		gmsh::model::geo::addCircleArc( 1,  9,  2,  9);
		gmsh::model::geo::addCircleArc( 3, 10,  4, 10);
		gmsh::model::geo::addCircleArc( 5, 11,  6, 11);
		gmsh::model::geo::addCircleArc( 7, 12,  8, 12);
		gmsh::model::geo::addCircleArc(13, 21, 14, 13);
		gmsh::model::geo::addCircleArc(15, 22, 16, 14);
		gmsh::model::geo::addCircleArc(17, 23, 18, 15);
		gmsh::model::geo::addCircleArc(19, 24, 20, 16);
		//loops
		gmsh::model::geo::addCurveLoop({ 9, 1, 10, 2, 11, 3, 12, 4}, 1);
		gmsh::model::geo::addCurveLoop({13, 5, 14, 6, 15, 7, 16, 8}, 2);
		//surfaces
		gmsh::model::geo::addPlaneSurface({2, 1}, 1);
	}

	void RHS::setup_isolated(void)
	{
		if(m_radius_inner == 0 && m_radius_outer != 0) m_isolated = { 8,  9, 10, 11};
		if(m_radius_inner != 0 && m_radius_outer == 0) m_isolated = {12, 13, 14, 15};
		if(m_radius_inner != 0 && m_radius_outer != 0) m_isolated = { 8,  9, 10, 11, 20, 21, 22, 23};
	}
	void RHS::setup_geometry(void) const
	{
		m_radius_inner == 0 ? 
			m_radius_outer == 0 ? setup_geometry_1() : setup_geometry_2() :
			m_radius_outer == 0 ? setup_geometry_3() : setup_geometry_4();
	}
}