//gmsh
#include <gmsh.h>

//Sections
#include "Sections/inc/Rectangle.hpp"

namespace sections
{
	//constructor
	Rectangle::Rectangle(void) : m_width{2.00e-01}, m_height{6.00e-01}, m_fibers_width{10}, m_fibers_height{10}
	{
		return;
	}

	//destructor
	Rectangle::~Rectangle(void)
	{
		return;
	}

	//data
	double Rectangle::width(void) const
	{
		return m_width;
	}
	double Rectangle::width(double width)
	{
		return m_width = width;
	}

	double Rectangle::height(void) const
	{
		return m_height;
	}
	double Rectangle::height(double height)
	{
		return m_height = height;
	}

	uint32_t Rectangle::fibers_width(void) const
	{
		return m_fibers_width;
	}
	uint32_t Rectangle::fibers_width(uint32_t fibers_width)
	{
		return m_fibers_width = fibers_width;
	}

	uint32_t Rectangle::fibers_height(void) const
	{
		return m_fibers_height;
	}
	uint32_t Rectangle::fibers_height(uint32_t fibers_height)
	{
		return m_fibers_height = fibers_height;
	}

	//geometry
	void Rectangle::setup_fibers(void)
	{
		fibers::Fiber::create_rectangles(m_fibers, -m_width / 2, -m_height / 2, m_width, m_height, m_fibers_width, m_fibers_height);
	}
	void Rectangle::setup_geometry(void) const
	{
		//data
		const double w = m_width;
		const double h = m_height;
		//points
		gmsh::model::geo::addPoint(0, 0, 0, m_mesh_size, 1);
		gmsh::model::geo::addPoint(w, 0, 0, m_mesh_size, 2);
		gmsh::model::geo::addPoint(w, h, 0, m_mesh_size, 3);
		gmsh::model::geo::addPoint(0, h, 0, m_mesh_size, 4);
		//curves
		gmsh::model::geo::addLine(1, 2, 1);
		gmsh::model::geo::addLine(2, 3, 2);
		gmsh::model::geo::addLine(3, 4, 3);
		gmsh::model::geo::addLine(4, 1, 4);
		//loops
		gmsh::model::geo::addCurveLoop({1, 2, 3, 4}, 1);
		//surfaces
		gmsh::model::geo::addPlaneSurface({1}, 1);
	}
}