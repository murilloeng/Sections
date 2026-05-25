//gmsh
#include <gmsh.h>

//Sections
#include "Sections/inc/ProfileI.hpp"

namespace sections
{
	//constructor
	ProfileI::ProfileI(void)
	{
		return;
	}
	
	//destructor
	ProfileI::~ProfileI(void)
	{
		return;
	}

	//geometry
	void ProfileI::setup_geometry_1(void) const
	{
		//data
		const double hw = m_web_height;
		const double wf = m_flange_width;
		const double tw = m_web_thickness;
		const double tf = m_flange_thickness;
		//points
		gmsh::model::geo::addPoint(+wf / 2, -hw / 2, 0, m_mesh_size,  3);
		gmsh::model::geo::addPoint(+tw / 2, -hw / 2, 0, m_mesh_size,  4);
		gmsh::model::geo::addPoint(+tw / 2, +hw / 2, 0, m_mesh_size,  5);
		gmsh::model::geo::addPoint(+wf / 2, +hw / 2, 0, m_mesh_size,  6);
		gmsh::model::geo::addPoint(-wf / 2, +hw / 2, 0, m_mesh_size,  9);
		gmsh::model::geo::addPoint(-tw / 2, +hw / 2, 0, m_mesh_size, 10);
		gmsh::model::geo::addPoint(-tw / 2, -hw / 2, 0, m_mesh_size, 11);
		gmsh::model::geo::addPoint(-wf / 2, -hw / 2, 0, m_mesh_size, 12);
		gmsh::model::geo::addPoint(-wf / 2, -hw / 2 - tf, 0, m_mesh_size, 1);
		gmsh::model::geo::addPoint(+wf / 2, -hw / 2 - tf, 0, m_mesh_size, 2);
		gmsh::model::geo::addPoint(+wf / 2, +hw / 2 + tf, 0, m_mesh_size, 7);
		gmsh::model::geo::addPoint(-wf / 2, +hw / 2 + tf, 0, m_mesh_size, 8);
		//curves
		gmsh::model::geo::addLine( 1,  2,  1);
		gmsh::model::geo::addLine( 2,  3,  2);
		gmsh::model::geo::addLine( 3,  4,  3);
		gmsh::model::geo::addLine( 4,  5,  4);
		gmsh::model::geo::addLine( 5,  6,  5);
		gmsh::model::geo::addLine( 6,  7,  6);
		gmsh::model::geo::addLine( 7,  8,  7);
		gmsh::model::geo::addLine( 8,  9,  8);
		gmsh::model::geo::addLine( 9, 10,  9);
		gmsh::model::geo::addLine(10, 11, 10);
		gmsh::model::geo::addLine(11, 12, 11);
		gmsh::model::geo::addLine(12,  1, 12);
		//loops
		gmsh::model::geo::addCurveLoop({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, 1);
		//surfaces
		gmsh::model::geo::addPlaneSurface({1}, 1);
	}
	void ProfileI::setup_geometry_2(void) const
	{
		//data
		const double r = m_radius;
		const double hw = m_web_height;
		const double wf = m_flange_width;
		const double tw = m_web_thickness;
		const double tf = m_flange_thickness;
		//points
		gmsh::model::geo::addPoint(-wf / 2, -hw / 2 - tf, 0, m_mesh_size, 1);
		gmsh::model::geo::addPoint(+wf / 2, -hw / 2 - tf, 0, m_mesh_size, 2);
		gmsh::model::geo::addPoint(+wf / 2, -hw / 2, 0, m_mesh_size, 3);
		gmsh::model::geo::addPoint(+tw / 2 + r, -hw / 2, 0, m_mesh_size, 4);
		gmsh::model::geo::addPoint(+tw / 2 + r, -hw / 2 + r, 0, m_mesh_size, 5);
		gmsh::model::geo::addPoint(+tw / 2, -hw / 2 + r, 0, m_mesh_size, 6);
		gmsh::model::geo::addPoint(+tw / 2, +hw / 2 - r, 0, m_mesh_size, 7);
		gmsh::model::geo::addPoint(+tw / 2 + r, +hw / 2 - r, 0, m_mesh_size, 8);
		gmsh::model::geo::addPoint(+tw / 2 + r, +hw / 2, 0, m_mesh_size, 9);
		gmsh::model::geo::addPoint(+wf / 2, +hw / 2, 0, m_mesh_size, 10);
		gmsh::model::geo::addPoint(+wf / 2, +hw / 2 + tf, 0, m_mesh_size, 11);
		gmsh::model::geo::addPoint(-wf / 2, +hw / 2 + tf, 0, m_mesh_size, 12);
		gmsh::model::geo::addPoint(-wf / 2, +hw / 2, 0, m_mesh_size, 13);
		gmsh::model::geo::addPoint(-tw / 2 - r, +hw / 2, 0, m_mesh_size, 14);
		gmsh::model::geo::addPoint(-tw / 2 - r, +hw / 2 - r, 0, m_mesh_size, 15);
		gmsh::model::geo::addPoint(-tw / 2, +hw / 2 - r, 0, m_mesh_size, 16);
		gmsh::model::geo::addPoint(-tw / 2, -hw / 2 + r, 0, m_mesh_size, 17);
		gmsh::model::geo::addPoint(-tw / 2 - r, -hw / 2 + r, 0, m_mesh_size, 18);
		gmsh::model::geo::addPoint(-tw / 2 - r, -hw / 2, 0, m_mesh_size, 19);
		gmsh::model::geo::addPoint(-wf / 2, -hw / 2, 0, m_mesh_size, 20);
		//curves
		gmsh::model::geo::addLine( 1,  2,  1);
		gmsh::model::geo::addLine( 2,  3,  2);
		gmsh::model::geo::addLine( 3,  4,  3);
		gmsh::model::geo::addLine( 6,  7,  5);
		gmsh::model::geo::addLine( 9, 10,  7);
		gmsh::model::geo::addLine(10, 11,  8);
		gmsh::model::geo::addLine(11, 12,  9);
		gmsh::model::geo::addLine(12, 13, 10);
		gmsh::model::geo::addLine(13, 14, 11);
		gmsh::model::geo::addLine(16, 17, 13);
		gmsh::model::geo::addLine(19, 20, 15);
		gmsh::model::geo::addLine(20,  1, 16);
		gmsh::model::geo::addCircleArc( 4,  5,  6,  4);
		gmsh::model::geo::addCircleArc( 7,  8,  9,  6);
		gmsh::model::geo::addCircleArc(14, 15, 16, 12);
		gmsh::model::geo::addCircleArc(17, 18, 19, 14);
		//loops
		gmsh::model::geo::addCurveLoop({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}, 1);
		//surfaces
		gmsh::model::geo::addPlaneSurface({1}, 1);
	}

	void ProfileI::setup_isolated(void)
	{
		if(m_radius) m_isolated = {4, 7, 14, 17};
	}
	void ProfileI::setup_geometry(void) const
	{
		m_radius == 0 ? setup_geometry_1() : setup_geometry_2();
	}
}