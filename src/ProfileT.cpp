//gmsh
#include <gmsh.h>

//Sections
#include "Sections/inc/ProfileT.hpp"

namespace sections
{
	//constructor
	ProfileT::ProfileT(void) : 
		m_radius{1.00e-02}, m_web_height{1.00e-01}, m_flange_width{1.00e-01}, m_web_thickness{1.00e-02}, m_flange_thickness{1.00e-02}
	{
		return;
	}
	
	//destructor
	ProfileT::~ProfileT(void)
	{
		return;
	}

	//data
	double ProfileT::radius(void) const
	{
		return m_radius;
	}
	double ProfileT::radius(double radius)
	{
		return m_radius = radius;
	}

	double ProfileT::web_height(void) const
	{
		return m_web_height;
	}
	double ProfileT::web_height(double web_height)
	{
		return m_web_height = web_height;
	}

	double ProfileT::flange_width(void) const
	{
		return m_flange_width;
	}
	double ProfileT::flange_width(double flange_width)
	{
		return m_flange_width = flange_width;
	}

	double ProfileT::web_thickness(void) const
	{
		return m_web_thickness;
	}
	double ProfileT::web_thickness(double web_thickness)
	{
		return m_web_thickness = web_thickness;
	}

	double ProfileT::flange_thickness(void) const
	{
		return m_flange_thickness;
	}
	double ProfileT::flange_thickness(double flange_thickness)
	{
		return m_flange_thickness = flange_thickness;
	}

	//geometry
	void ProfileT::setup_geometry_1(void) const
	{
		//data
		const double hw = m_web_height;
		const double wf = m_flange_width;
		const double tw = m_web_thickness;
		const double tf = m_flange_thickness;
		//points
		gmsh::model::geo::addPoint(-tw / 2, 0, 0, m_mesh_size, 1);
		gmsh::model::geo::addPoint(+tw / 2, 0, 0, m_mesh_size, 2);
		gmsh::model::geo::addPoint(-wf / 2, hw, 0, m_mesh_size, 3);
		gmsh::model::geo::addPoint(-tw / 2, hw, 0, m_mesh_size, 4);
		gmsh::model::geo::addPoint(+tw / 2, hw, 0, m_mesh_size, 5);
		gmsh::model::geo::addPoint(+wf / 2, hw, 0, m_mesh_size, 6);
		gmsh::model::geo::addPoint(-wf / 2, hw + tf, 0, m_mesh_size, 7);
		gmsh::model::geo::addPoint(+wf / 2, hw + tf, 0, m_mesh_size, 8);
		//curves
		gmsh::model::geo::addLine(1, 2, 1);
		gmsh::model::geo::addLine(2, 5, 2);
		gmsh::model::geo::addLine(5, 6, 3);
		gmsh::model::geo::addLine(6, 8, 4);
		gmsh::model::geo::addLine(8, 7, 5);
		gmsh::model::geo::addLine(7, 3, 6);
		gmsh::model::geo::addLine(3, 4, 7);
		gmsh::model::geo::addLine(4, 1, 8);
		//loops
		gmsh::model::geo::addCurveLoop({1, 2, 3, 4, 5, 6, 7, 8}, 1);
		//surfaces
		gmsh::model::geo::addPlaneSurface({1}, 1);
	}
	void ProfileT::setup_geometry_2(void) const
	{
		//data
		const double r = m_radius;
		const double hw = m_web_height;
		const double wf = m_flange_width;
		const double tw = m_web_thickness;
		const double tf = m_flange_thickness;
		//points
		gmsh::model::geo::addPoint(-tw / 2, 0, 0, m_mesh_size, 1);
		gmsh::model::geo::addPoint(+tw / 2, 0, 0, m_mesh_size, 2);
		gmsh::model::geo::addPoint(-wf / 2, hw, 0, m_mesh_size, 9);
		gmsh::model::geo::addPoint(+wf / 2, hw, 0, m_mesh_size, 6);
		gmsh::model::geo::addPoint(-tw / 2, hw - r, 0, m_mesh_size, 12);
		gmsh::model::geo::addPoint(+tw / 2, hw - r, 0, m_mesh_size,  3);
		gmsh::model::geo::addPoint(-tw / 2 - r, hw, 0, m_mesh_size, 10);
		gmsh::model::geo::addPoint(+tw / 2 + r, hw, 0, m_mesh_size,  5);
		gmsh::model::geo::addPoint(-wf / 2, hw + tf, 0, m_mesh_size, 8);
		gmsh::model::geo::addPoint(+wf / 2, hw + tf, 0, m_mesh_size, 7);
		gmsh::model::geo::addPoint(-tw / 2 - r, hw - r, 0, m_mesh_size, 11);
		gmsh::model::geo::addPoint(+tw / 2 + r, hw - r, 0, m_mesh_size,  4);
		//curves
		gmsh::model::geo::addLine( 1,  2,  1);
		gmsh::model::geo::addLine( 2,  3,  2);
		gmsh::model::geo::addLine( 5,  6,  4);
		gmsh::model::geo::addLine( 6,  7,  5);
		gmsh::model::geo::addLine( 7,  8,  6);
		gmsh::model::geo::addLine( 8,  9,  7);
		gmsh::model::geo::addLine( 9, 10,  8);
		gmsh::model::geo::addLine(12,  1, 10);
		gmsh::model::geo::addCircleArc( 3,  4,  5, 3);
		gmsh::model::geo::addCircleArc(10, 11, 12, 9);
		//loops
		gmsh::model::geo::addCurveLoop({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 1);
		//surfaces
		gmsh::model::geo::addPlaneSurface({1}, 1);
	}
	void ProfileT::setup_geometry(void) const
	{
		m_radius == 0 ? setup_geometry_1() : setup_geometry_2();
	}
}