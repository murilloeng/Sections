//Sections
#include "Sections/inc/Section.hpp"

//Test
#include "Sections/Test/inc/Draw.hpp"

//Canvas
#include "Canvas/inc/Scene/Scene.hpp"
#include "Canvas/inc/Vertices/Model3D.hpp"

//constructor
Draw::Draw(void)
{
	return;
}

//constructor
Draw::~Draw(void)
{
	return;
}

//data
What& Draw::what(void)
{
	return m_what;
}

sections::Section* Draw::section(void)
{
	return m_section;
}
sections::Section* Draw::section(sections::Section* section)
{
	return m_section = section;
}

//draw
void Draw::draw(void)
{
	//data
	const uint64_t offset_1 = m_counter_points * sizeof(uint32_t);
	const uint64_t offset_2 = offset_1 + m_counter_lines * sizeof(uint32_t);
	//draw
	m_vao.bind();
	m_shader.bind();
	glDrawElements(GL_POINTS, m_counter_points, GL_UNSIGNED_INT, nullptr);
	glDrawElements(GL_LINES, m_counter_lines, GL_UNSIGNED_INT, (void*) offset_1);
	glDrawElements(GL_TRIANGLES, m_counter_triangles, GL_UNSIGNED_INT, (void*) offset_2);
}
void Draw::setup(void)
{
	//data
	m_counter_lines = 0;
	m_counter_points = 0;
	m_counter_vertices = 0;
	m_counter_triangles = 0;
	//setup
	if(m_what.nodes()) setup_nodes();
	if(m_what.elements()) setup_elements();
	//allocate
	m_vbo.allocate(m_counter_vertices);
	m_ibo.allocate(m_counter_points + m_counter_lines + m_counter_triangles);
}
void Draw::update(void)
{
	//data
	m_index_lines = 0;
	m_index_points = 0;
	m_index_vertices = 0;
	m_index_triangles = 0;
	//update
	if(m_what.nodes()) update_nodes();
	if(m_what.elements()) update_elements();
	//transfer
	m_vbo.transfer();
	m_ibo.transfer();
}

//setup
void Draw::setup_nodes(void)
{
	//data
	const uint64_t nn = m_section->nodes().size();
	//setup
	m_counter_points += nn;
	m_counter_vertices += nn;
}
void Draw::setup_elements(void)
{
	//data
	const uint64_t nn = m_section->nodes().size();
	const uint64_t ne = m_section->elements().size();
	//setup
	m_counter_vertices += nn;
	m_counter_lines += 2 * ne;
}

//update
void Draw::update_nodes(void)
{
	//data
	const uint64_t nn = m_section->nodes().size();
	uint32_t* ibo_ptr = m_ibo.data() + m_index_points;
	const std::vector<sections::Node>& nodes = m_section->nodes();
	canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) m_vbo.data() + m_index_vertices;
	//buffers data
	for(uint32_t i = 0; i < nn; i++)
	{
		ibo_ptr[i] = i;
		vbo_ptr[i].m_color = "red";
		vbo_ptr[i].m_position = nodes[i].position();
	}
	//update
	m_index_points += nn;
	m_index_vertices += nn;
}
void Draw::update_elements(void)
{
	//data
	const uint64_t nn = m_section->nodes().size();
	const uint64_t ne = m_section->elements().size();
	const std::vector<sections::Node>& nodes = m_section->nodes();
	const std::vector<sections::Element>& elements = m_section->elements();
	uint32_t* ibo_ptr = m_ibo.data() + m_counter_points + m_index_lines;
	canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) m_vbo.data() + m_index_vertices;
	//vbo data
	for(uint32_t i = 0; i < nn; i++)
	{
		vbo_ptr[i].m_color = "blue";
		vbo_ptr[i].m_position = nodes[i].position();
	}
	//ibo data
	for(uint32_t i = 0; i < ne; i++)
	{
		ibo_ptr[2 * i + 0] = m_index_vertices + elements[i].node(0);
		ibo_ptr[2 * i + 1] = m_index_vertices + elements[i].node(1);
	}
	//update
	m_index_vertices += nn;
	m_index_lines += 2 * ne;
}