//Sections
#include "Sections/inc/Section.hpp"
#include "Sections/Test/inc/Draw.hpp"
#include "Sections/inc/Fibers/Rectangle.hpp"

//Canvas
#include "Canvas/inc/Scene/Scene.hpp"
#include "Canvas/inc/Vertices/Model3D.hpp"
#include "Canvas/inc/Objects/Tessellator.hpp"

//constructor
Draw::Draw(void) : m_type{Type::Fibers}
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

Draw::Type Draw::type(Type type)
{
	return m_type = type;
}
Draw::Type Draw::type(void) const
{
	return m_type;
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
	if(m_type == Type::Mesh)
	{
		if(m_what.nodes()) setup_nodes();
		if(m_what.elements()) setup_elements();
	}
	else if(m_type == Type::Fibers)
	{
		if(m_what.fibers()) setup_fibers();
	}
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
	if(m_type == Type::Mesh)
	{
		if(m_what.nodes()) update_nodes();
		if(m_what.elements()) update_elements();
	}
	else if(m_type == Type::Fibers)
	{
		if(m_what.fibers()) update_fibers();
	}
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
void Draw::setup_fibers(void)
{
	for(const sections::fibers::Fiber* fiber : m_section->fibers())
	{
		if(dynamic_cast<const sections::fibers::Rectangle*>(fiber))
		{
			m_counter_lines += 8;
			m_counter_vertices += 8;
			m_counter_triangles += 6;
		}
	}
}
void Draw::setup_elements(void)
{
	//data
	const uint64_t nn = m_section->nodes().size();
	const uint64_t ne = m_section->elements().size();
	//setup
	m_counter_lines += 12 * ne;
	m_counter_vertices += 2 * nn;
	m_counter_triangles += 12 * ne;
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
		vbo_ptr[i].m_position[2] = 0;
		vbo_ptr[i].m_position[0] = -nodes[i].position(1);
		vbo_ptr[i].m_position[1] = +nodes[i].position(0);
	}
	//update
	m_index_points += nn;
	m_index_vertices += nn;
}
void Draw::update_fibers(void)
{
	//data
	uint32_t* ibo_ptr_lines = m_ibo.data() + m_counter_points + m_index_lines;
	uint32_t* ibo_ptr_triangles = m_ibo.data() + m_counter_points + m_counter_lines + m_index_triangles;
	canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) m_vbo.data() + m_index_vertices;
	//buffers data
	for(const sections::fibers::Fiber* fiber : m_section->fibers())
	{
		const double x2 = fiber->position(0);
		const double x3 = fiber->position(1);
		if(dynamic_cast<const sections::fibers::Rectangle*>(fiber))
		{
			//fiber
			sections::fibers::Rectangle* rectangle = (sections::fibers::Rectangle*) fiber;
			//vbo data
			const double s2 = rectangle->width();
			const double s3 = rectangle->height();
			for(uint32_t i = 0; i < 8; i++) vbo_ptr[i].m_position[2] = 0;
			for(uint32_t i = 0; i < 4; i++) vbo_ptr[i + 4].m_color = "blue";
			for(uint32_t i = 0; i < 4; i++) vbo_ptr[i + 0].m_color = "white";
			vbo_ptr[0].m_position[0] = vbo_ptr[3].m_position[0] = vbo_ptr[4].m_position[0] = vbo_ptr[7].m_position[0] = x2 - s2 / 2;
			vbo_ptr[1].m_position[0] = vbo_ptr[2].m_position[0] = vbo_ptr[5].m_position[0] = vbo_ptr[6].m_position[0] = x2 + s2 / 2;
			vbo_ptr[0].m_position[1] = vbo_ptr[1].m_position[1] = vbo_ptr[4].m_position[1] = vbo_ptr[5].m_position[1] = x3 - s3 / 2;
			vbo_ptr[2].m_position[1] = vbo_ptr[3].m_position[1] = vbo_ptr[6].m_position[1] = vbo_ptr[7].m_position[1] = x3 + s3 / 2;
			//ibo data
			ibo_ptr_triangles[3 * 0 + 0] = m_index_vertices + 4;
			ibo_ptr_triangles[3 * 0 + 1] = m_index_vertices + 5;
			ibo_ptr_triangles[3 * 0 + 2] = m_index_vertices + 6;
			ibo_ptr_triangles[3 * 1 + 0] = m_index_vertices + 4;
			ibo_ptr_triangles[3 * 1 + 1] = m_index_vertices + 6;
			ibo_ptr_triangles[3 * 1 + 2] = m_index_vertices + 7;
			ibo_ptr_lines[2 * 0 + 0] = ibo_ptr_lines[2 * 3 + 1] = m_index_vertices + 0;
			ibo_ptr_lines[2 * 1 + 0] = ibo_ptr_lines[2 * 0 + 1] = m_index_vertices + 1;
			ibo_ptr_lines[2 * 2 + 0] = ibo_ptr_lines[2 * 1 + 1] = m_index_vertices + 2;
			ibo_ptr_lines[2 * 3 + 0] = ibo_ptr_lines[2 * 2 + 1] = m_index_vertices + 3;
			//update
			vbo_ptr += 8;
			ibo_ptr_lines += 8;
			m_index_lines += 8;
			m_index_vertices += 8;
			ibo_ptr_triangles += 6;
			m_index_triangles += 6;
		}
	}
}
void Draw::update_elements(void)
{
	//data
	uint32_t triangles[12];
	canvas::vec2 vertices[6];
	const uint32_t loops[] = {0, 6};
	const uint32_t list[] = {0, 3, 1, 4, 2, 5};
	const uint64_t nn = m_section->nodes().size();
	const uint64_t ne = m_section->elements().size();
	const std::vector<sections::Node>& nodes = m_section->nodes();
	const std::vector<sections::Element>& elements = m_section->elements();
	uint32_t* ibo_ptr_lines = m_ibo.data() + m_counter_points + m_index_lines;
	uint32_t* ibo_ptr_triangles = m_ibo.data() + m_counter_points + m_counter_lines + m_index_triangles;
	canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) m_vbo.data() + m_index_vertices;
	//vbo data
	for(uint32_t i = 0; i < nn; i++)
	{
		vbo_ptr[i + 1 * nn].m_color = "blue";
		vbo_ptr[i + 0 * nn].m_color = "white";
		vbo_ptr[i + 0 * nn].m_position[2] = 0;
		vbo_ptr[i + 1 * nn].m_position[2] = 0;
		vbo_ptr[i + 0 * nn].m_position[0] = -nodes[i].position(1);
		vbo_ptr[i + 0 * nn].m_position[1] = +nodes[i].position(0);
		vbo_ptr[i + 1 * nn].m_position[0] = -nodes[i].position(1);
		vbo_ptr[i + 1 * nn].m_position[1] = +nodes[i].position(0);
	}
	//ibo data
	for(uint32_t i = 0; i < ne; i++)
	{
		for(uint32_t j = 0; j < 6; j++)
		{
			vertices[j][0] = vbo_ptr[elements[i].node(list[j])].m_position[0];
			vertices[j][1] = vbo_ptr[elements[i].node(list[j])].m_position[1];
			ibo_ptr_lines[12 * i + 2 * j + 0] = m_index_vertices + elements[i].node(list[(j + 0) % 6]);
			ibo_ptr_lines[12 * i + 2 * j + 1] = m_index_vertices + elements[i].node(list[(j + 1) % 6]);
		}
		canvas::objects::Tessellator(vertices, loops, 1, triangles).tessellate();
		for(uint32_t j = 0; j < 4; j++)
		{
			ibo_ptr_triangles[12 * i + 3 * j + 0] = m_index_vertices + nn + elements[i].node(list[triangles[3 * j + 0]]);
			ibo_ptr_triangles[12 * i + 3 * j + 1] = m_index_vertices + nn + elements[i].node(list[triangles[3 * j + 1]]);
			ibo_ptr_triangles[12 * i + 3 * j + 2] = m_index_vertices + nn + elements[i].node(list[triangles[3 * j + 2]]);
		}
	}
	//update
	m_index_lines += 12 * ne;
	m_index_vertices += 2 * nn;
	m_index_triangles += 12 * ne;
}