//gmsh
#include <gmsh.h>

//Sections
#include "Sections/inc/ProfileC.hpp"

namespace sections
{
	//constructor
	ProfileC::ProfileC(void)
	{
		return;
	}

	//destructor
	ProfileC::~ProfileC(void)
	{
		return;
	}

	//geometry
	void ProfileC::setup_geometry_1(void) const
	{
		//data
		const double hw = m_web_height;
		const double wf = m_flange_width;
		const double tw = m_web_thickness;
		const double tf = m_flange_thickness;
		//points
		gmsh::model::geo::addPoint(wf, -hw / 2, 0, m_mesh_size, 3);
		gmsh::model::geo::addPoint(tw, -hw / 2, 0, m_mesh_size, 4);
		gmsh::model::geo::addPoint(tw, +hw / 2, 0, m_mesh_size, 5);
		gmsh::model::geo::addPoint(wf, +hw / 2, 0, m_mesh_size, 6);
		gmsh::model::geo::addPoint(0, -hw / 2 - tf, 0, m_mesh_size, 1);
		gmsh::model::geo::addPoint(0, +hw / 2 + tf, 0, m_mesh_size, 8);
		gmsh::model::geo::addPoint(wf, -hw / 2 - tf, 0, m_mesh_size, 2);
		gmsh::model::geo::addPoint(wf, +hw / 2 + tf, 0, m_mesh_size, 7);
		//curves
		gmsh::model::geo::addLine( 1,  2,  1);
		gmsh::model::geo::addLine( 2,  3,  2);
		gmsh::model::geo::addLine( 3,  4,  3);
		gmsh::model::geo::addLine( 4,  5,  4);
		gmsh::model::geo::addLine( 5,  6,  5);
		gmsh::model::geo::addLine( 6,  7,  6);
		gmsh::model::geo::addLine( 7,  8,  7);
		gmsh::model::geo::addLine( 8,  1,  8);
		//loops
		gmsh::model::geo::addCurveLoop({1, 2, 3, 4, 5, 6, 7, 8}, 1);
		//surfaces
		gmsh::model::geo::addPlaneSurface({1}, 1);
	}
	void ProfileC::setup_geometry_2(void) const
	{
		//data
		const double r = m_radius;
		const double hw = m_web_height;
		const double wf = m_flange_width;
		const double tw = m_web_thickness;
		const double tf = m_flange_thickness;
		//points
		gmsh::model::geo::addPoint(wf, -hw / 2, 0, m_mesh_size,  3);
		gmsh::model::geo::addPoint(wf, +hw / 2, 0, m_mesh_size, 10);
		gmsh::model::geo::addPoint(0, -hw / 2 - tf, 0, m_mesh_size,  1);
		gmsh::model::geo::addPoint(0, +hw / 2 + tf, 0, m_mesh_size, 12);
		gmsh::model::geo::addPoint(tw, -hw / 2 + r, 0, m_mesh_size,  6);
		gmsh::model::geo::addPoint(tw, +hw / 2 - r, 0, m_mesh_size,  7);
		gmsh::model::geo::addPoint(tw + r, -hw / 2, 0, m_mesh_size,  4);
		gmsh::model::geo::addPoint(tw + r, +hw / 2, 0, m_mesh_size,  9);
		gmsh::model::geo::addPoint(wf, -hw / 2 - tf, 0, m_mesh_size,  2);
		gmsh::model::geo::addPoint(wf, +hw / 2 + tf, 0, m_mesh_size, 11);
		gmsh::model::geo::addPoint(tw + r, -hw / 2 + r, 0, m_mesh_size, 5);
		gmsh::model::geo::addPoint(tw + r, +hw / 2 - r, 0, m_mesh_size, 8);
		//curves
		gmsh::model::geo::addLine( 1,  2,   1);
		gmsh::model::geo::addLine( 2,  3,   2);
		gmsh::model::geo::addLine( 3,  4,   3);
		gmsh::model::geo::addLine( 6,  7,   5);
		gmsh::model::geo::addLine( 9, 10,   7);
		gmsh::model::geo::addLine(10, 11,   8);
		gmsh::model::geo::addLine(11, 12,   9);
		gmsh::model::geo::addLine(12,  1,  10);
		gmsh::model::geo::addCircleArc( 4,  5,  6,  4);
		gmsh::model::geo::addCircleArc( 7,  8,  9,  6);
		//loops
		gmsh::model::geo::addCurveLoop({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 1);
		//surfaces
		gmsh::model::geo::addPlaneSurface({1}, 1);
	}

	void ProfileC::setup_fibers(void)
	{
		//data
		const double wh = m_web_height;
		const double fw = m_flange_width;
		const double wt = m_web_thickness;
		const double ft = m_flange_thickness;
		const uint32_t nwh = m_fibers_web_height;
		const uint32_t nfw = m_fibers_flange_width;
		const uint32_t nwt = m_fibers_web_thickness;
		const uint32_t nft = m_fibers_flange_thickness;
		//center
		const double S1 = fw / 2 * fw * ft;
		const double S2 = fw / 2 * fw * ft;
		const double S3 = (fw - wt / 2) * wh * wt;
		const double x3 = (S1 + S2 + S3) / (2 * fw * ft + wh * wt);
		//fibers
		fibers::Fiber::create_rectangles(m_fibers, -wh / 2 - ft, -x3, +ft, +fw, nft, nfw);
		fibers::Fiber::create_rectangles(m_fibers, +wh / 2 + ft, -x3, -ft, +fw, nft, nfw);
		fibers::Fiber::create_rectangles(m_fibers, -wh / 2, fw - x3 - wt, wh, wt, nwh, nwt);
	}
	void ProfileC::setup_isolated(void)
	{
		if(m_radius) m_isolated = {4, 7};
	}
	void ProfileC::setup_geometry(void) const
	{
		m_radius == 0 ? setup_geometry_1() : setup_geometry_2();
	}
}