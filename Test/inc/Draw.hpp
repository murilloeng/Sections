#pragma once

//Sections
#include "Sections/inc/Section.hpp"
#include "Sections/inc/Fibers/Arc.hpp"
#include "Sections/inc/Fibers/Rectangle.hpp"

//Test
#include "Sections/Test/inc/Type.hpp"
#include "Sections/Test/inc/What.hpp"

//Canvas
#include "Canvas/inc/Palettes/Palette.hpp"
#include "Canvas/inc/Objects/Bases/BaseModel3D.hpp"

class Engine;

class Draw : public canvas::objects::BaseModel3D
{
public:
	//constructor
	Draw(void);

	//destructor
	~Draw(void);

	//data
	What& what(void);

	Type type(Type);
	Type type(void) const;
	
	void extrema(double&, double&) const;

	sections::Section* section(void);
	sections::Section* section(sections::Section*);

private:
	//draw
	void draw(void) override;
	void setup(void) override;
	void update(void) override;

	//compute
	void compute_extrema(void);

	//setup
	void setup_nodes(void);
	void setup_fibers(void);
	void setup_elements(void);
	void setup_fiber_arc(void);
	void setup_fiber_rectangle(void);

	//update
	void update_nodes(void);
	void update_fibers(void);
	void update_elements(void);
	void update_fiber_arc(const sections::fibers::Arc*);
	void update_fiber_rectangle(const sections::fibers::Rectangle*);

	//height
	double node_height(const sections::Node*) const;
	double fiber_height(const sections::fibers::Fiber*) const;

	//colors
	canvas::Color node_color(const sections::Node*) const;
	canvas::Color fiber_color(const sections::fibers::Fiber*) const;

	//data
	Type m_type;
	What m_what;
	double m_ue[6], m_ge[12];
	
	uint32_t m_index_lines;
	uint32_t m_index_points;
	uint32_t m_index_vertices;
	uint32_t m_index_triangles;
	
	uint32_t m_counter_lines;
	uint32_t m_counter_points;
	uint32_t m_counter_vertices;
	uint32_t m_counter_triangles;
	
	sections::Section* m_section;
	const canvas::palettes::Palette* m_palette;

	double m_scale[uint32_t(Type::Last)];

	//friends
	friend class Engine;
};