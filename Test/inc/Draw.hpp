#pragma once

//Sections
#include "Sections/inc/Section.hpp"
#include "Sections/Test/inc/What.hpp"
#include "Sections/inc/Fibers/Arc.hpp"
#include "Sections/inc/Fibers/Rectangle.hpp"

//Canvas
#include "Canvas/inc/Objects/Bases/BaseModel3D.hpp"

class Draw : public canvas::objects::BaseModel3D
{
public:
	//constructor
	Draw(void);

	//destructor
	~Draw(void);

	//enums
	enum class Type : uint32_t { Mesh, Fibers, Last };

	//data
	What& what(void);

	Type type(Type);
	Type type(void) const;

	sections::Section* section(void);
	sections::Section* section(sections::Section*);

private:
	//draw
	void draw(void) override;
	void setup(void) override;
	void update(void) override;

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

	//data
	Type m_type;
	What m_what;
	sections::Section* m_section;

	uint32_t m_index_lines;
	uint32_t m_index_points;
	uint32_t m_index_vertices;
	uint32_t m_index_triangles;

	uint32_t m_counter_lines;
	uint32_t m_counter_points;
	uint32_t m_counter_vertices;
	uint32_t m_counter_triangles;
};