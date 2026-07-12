#pragma once

//Sections
#include "Sections/inc/Section.hpp"
#include "Sections/Test/inc/What.hpp"

//Canvas
#include "Canvas/inc/Objects/Bases/BaseModel3D.hpp"

class Draw : public canvas::objects::BaseModel3D
{
public:
	//constructor
	Draw(void);

	//destructor
	~Draw(void);

	//data
	What& what(void);

	sections::Section* section(void);
	sections::Section* section(sections::Section*);

private:
	//draw
	void draw(void) override;
	void setup(void) override;
	void update(void) override;

	//setup
	void setup_nodes(void);
	void setup_elements(void);

	//update
	void update_nodes(void);
	void update_elements(void);

	//data
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