//std
#include <cmath>
#include <cfloat>
#include <algorithm>

//Sections
#include "Sections/inc/Section.hpp"
#include "Sections/Test/inc/Draw.hpp"
#include "Sections/inc/Fibers/Arc.hpp"
#include "Sections/inc/Fibers/Rectangle.hpp"

//Canvas
#include "Canvas/inc/Scene/Scene.hpp"
#include "Canvas/inc/Vertices/Model3D.hpp"
#include "Canvas/inc/Palettes/Palette.hpp"
#include "Canvas/inc/Objects/Tessellator.hpp"
#include "Canvas/inc/Objects/Image/Palette.hpp"

//data
static const uint32_t mesh_curved = 10;

//constructor
Draw::Draw(void) : m_type{Type::FibersGradientS33}
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

Type Draw::type(Type type)
{
	return m_type = type;
}
Type Draw::type(void) const
{
	return m_type;
}

void Draw::extrema(double& s1, double& s2) const
{
	//data
	const Type tmw[] = {
		Type::MeshWarpingT, Type::MeshWarpingS2, Type::MeshWarpingS3
	};
	const Type tfw[] = {
		Type::FibersWarpingT, Type::FibersWarpingS2, Type::FibersWarpingS3
	};
	const Type tmg[] = {
		Type::MeshGradientT2, Type::MeshGradientS22, Type::MeshGradientS32, Type::MeshGradientT3, Type::MeshGradientS23, Type::MeshGradientS33
	};
	const Type tfg[] = {
		Type::FibersGradientT2, Type::FibersGradientS22, Type::FibersGradientS32, Type::FibersGradientT3, Type::FibersGradientS23, Type::FibersGradientS33
	};
	//warping
	for(uint32_t i = 0; i < 3; i++)
	{
		if(m_type == tmw[i] || m_type == tfw[i])
		{
			s1 = m_ue[i + 0];
			s2 = m_ue[i + 3];
		}
	}
	//gradient
	for(uint32_t i = 0; i < 6; i++)
	{
		if(m_type == tmg[i] || m_type == tfg[i])
		{
			s1 = m_ge[i + 0];
			s2 = m_ge[i + 6];
		}
	}
}

sections::Section* Draw::section(void)
{
	return m_section;
}
sections::Section* Draw::section(sections::Section* section)
{
	m_section = section;
	compute_extrema();
	return section;
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
	const Type tm[] = {
		Type::Mesh, Type::MeshWarpingT, Type::MeshWarpingS2, Type::MeshWarpingS3,
		Type::MeshGradientT2, Type::MeshGradientS22, Type::MeshGradientS32, Type::MeshGradientT3, Type::MeshGradientS23, Type::MeshGradientS33
	};
	const Type tf[] = {
		Type::Fibers, Type::FibersWarpingT, Type::FibersWarpingS2, Type::FibersWarpingS3,
		Type::FibersGradientT2, Type::FibersGradientS22, Type::FibersGradientS32, Type::FibersGradientT3, Type::FibersGradientS23, Type::FibersGradientS33
	};
	//setup
	if(std::find(tm, tm + 10, m_type) != tm + 10)
	{
		if(m_what.nodes()) setup_nodes();
		if(m_what.elements()) setup_elements();
	}
	else if(std::find(tf, tf + 10, m_type) != tf + 10)
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
	const Type tm[] = {
		Type::Mesh, Type::MeshWarpingT, Type::MeshWarpingS2, Type::MeshWarpingS3,
		Type::MeshGradientT2, Type::MeshGradientS22, Type::MeshGradientS32, Type::MeshGradientT3, Type::MeshGradientS23, Type::MeshGradientS33
	};
	const Type tf[] = {
		Type::Fibers, Type::FibersWarpingT, Type::FibersWarpingS2, Type::FibersWarpingS3,
		Type::FibersGradientT2, Type::FibersGradientS22, Type::FibersGradientS32, Type::FibersGradientT3, Type::FibersGradientS23, Type::FibersGradientS33
	};
	//update
	if(std::find(tm, tm + 10, m_type) != tm + 10)
	{
		if(m_what.nodes()) update_nodes();
		if(m_what.elements()) update_elements();
	}
	else if(std::find(tf, tf + 10, m_type) != tf + 10)
	{
		if(m_what.fibers()) update_fibers();
	}
	//transfer
	m_vbo.transfer();
	m_ibo.transfer();
}

//compute
void Draw::compute_extrema(void)
{
	//setup
	for(uint32_t i = 0; i < 3; i++)
	{
		m_ue[i + 0] = m_ge[i + 0] = m_ge[i + 3] = +DBL_MAX;
		m_ue[i + 3] = m_ge[i + 6] = m_ge[i + 9] = -DBL_MAX;
	}
	//warping
	for(const sections::Node& node : m_section->nodes())
	{
		for(uint32_t i = 0; i < 3; i++)
		{
			m_ue[i + 0] = fmin(m_ue[i + 0], node.warping(i));
			m_ue[i + 3] = fmax(m_ue[i + 3], node.warping(i));
		}
	}
	//gradient
	for(const sections::fibers::Fiber* fiber : m_section->fibers())
	{
		for(uint32_t i = 0; i < 6; i++)
		{
			m_ge[i + 0] = fmin(m_ge[i + 0], fiber->gradient(i % 3, i / 3));
			m_ge[i + 6] = fmax(m_ge[i + 6], fiber->gradient(i % 3, i / 3));
		}
	}
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
		if(dynamic_cast<const sections::fibers::Arc*>(fiber)) setup_fiber_arc();
		if(dynamic_cast<const sections::fibers::Rectangle*>(fiber)) setup_fiber_rectangle();
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
void Draw::setup_fiber_arc(void)
{
	m_counter_lines += 4 * (mesh_curved + 1);
	m_counter_vertices += 4 * (mesh_curved + 1);
	m_counter_triangles += 6 * (mesh_curved + 0);
}
void Draw::setup_fiber_rectangle(void)
{
	m_counter_lines += 8;
	m_counter_vertices += 8;
	m_counter_triangles += 6;
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
	for(const sections::fibers::Fiber* fiber : m_section->fibers())
	{
		//cast
		const sections::fibers::Arc* arc = dynamic_cast<const sections::fibers::Arc*>(fiber);
		const sections::fibers::Rectangle* rectangle = dynamic_cast<const sections::fibers::Rectangle*>(fiber);
		//update
		if(arc) update_fiber_arc(arc);
		if(rectangle) update_fiber_rectangle(rectangle);
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
		vbo_ptr[i + 0 * nn].m_color = "white";
		vbo_ptr[i + 0 * nn].m_position[2] = 0;
		vbo_ptr[i + 1 * nn].m_position[2] = 0;
		vbo_ptr[i + 0 * nn].m_position[0] = -nodes[i].position(1);
		vbo_ptr[i + 0 * nn].m_position[1] = +nodes[i].position(0);
		vbo_ptr[i + 1 * nn].m_position[0] = -nodes[i].position(1);
		vbo_ptr[i + 1 * nn].m_position[1] = +nodes[i].position(0);
		vbo_ptr[i + 1 * nn].m_color = node_color(&m_section->node(i));
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
void Draw::update_fiber_arc(const sections::fibers::Arc* fiber)
{
	//data
	const double a = fiber->angle();
	const double q = fiber->start();
	const double r1 = fiber->radius(0);
	const double r2 = fiber->radius(1);
	uint32_t* ibo_ptr_lines = m_ibo.data() + m_counter_points + m_index_lines;
	uint32_t* ibo_ptr_triangles = m_ibo.data() + m_counter_points + m_counter_lines + m_index_triangles;
	canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) m_vbo.data() + m_index_vertices;
	//vbo data
	for(uint32_t i = 0; i <= mesh_curved; i++)
	{
		vbo_ptr[0 * (mesh_curved + 1) + i].m_color = "white";
		vbo_ptr[1 * (mesh_curved + 1) + i].m_color = "white";
		vbo_ptr[2 * (mesh_curved + 1) + i].m_color = fiber_color(fiber);
		vbo_ptr[3 * (mesh_curved + 1) + i].m_color = fiber_color(fiber);
		vbo_ptr[0 * (mesh_curved + 1) + i].m_position[2] = vbo_ptr[2 * (mesh_curved + 1) + i].m_position[2] = 0;
		vbo_ptr[1 * (mesh_curved + 1) + i].m_position[2] = vbo_ptr[3 * (mesh_curved + 1) + i].m_position[2] = 0;
		vbo_ptr[0 * (mesh_curved + 1) + i].m_position[0] = vbo_ptr[2 * (mesh_curved + 1) + i].m_position[0] = r1 * cos(q + a * i / mesh_curved);
		vbo_ptr[0 * (mesh_curved + 1) + i].m_position[1] = vbo_ptr[2 * (mesh_curved + 1) + i].m_position[1] = r1 * sin(q + a * i / mesh_curved);
		vbo_ptr[1 * (mesh_curved + 1) + i].m_position[0] = vbo_ptr[3 * (mesh_curved + 1) + i].m_position[0] = r2 * cos(q + a * i / mesh_curved);
		vbo_ptr[1 * (mesh_curved + 1) + i].m_position[1] = vbo_ptr[3 * (mesh_curved + 1) + i].m_position[1] = r2 * sin(q + a * i / mesh_curved);
	}
	//ibo data
	for(uint32_t i = 0; i < mesh_curved; i++)
	{
		ibo_ptr_lines[4 * i + 0] = m_index_vertices + 0 * (mesh_curved + 1) + i + 0;
		ibo_ptr_lines[4 * i + 1] = m_index_vertices + 0 * (mesh_curved + 1) + i + 1;
		ibo_ptr_lines[4 * i + 2] = m_index_vertices + 1 * (mesh_curved + 1) + i + 0;
		ibo_ptr_lines[4 * i + 3] = m_index_vertices + 1 * (mesh_curved + 1) + i + 1;
		ibo_ptr_triangles[6 * i + 0] = m_index_vertices + 2 * (mesh_curved + 1) + i + 0;
		ibo_ptr_triangles[6 * i + 1] = m_index_vertices + 3 * (mesh_curved + 1) + i + 0;
		ibo_ptr_triangles[6 * i + 2] = m_index_vertices + 3 * (mesh_curved + 1) + i + 1;
		ibo_ptr_triangles[6 * i + 3] = m_index_vertices + 2 * (mesh_curved + 1) + i + 0;
		ibo_ptr_triangles[6 * i + 4] = m_index_vertices + 3 * (mesh_curved + 1) + i + 1;
		ibo_ptr_triangles[6 * i + 5] = m_index_vertices + 2 * (mesh_curved + 1) + i + 1;
	}
	ibo_ptr_lines[4 * mesh_curved + 0] = m_index_vertices + 0 * (mesh_curved + 1);
	ibo_ptr_lines[4 * mesh_curved + 1] = m_index_vertices + 1 * (mesh_curved + 1);
	ibo_ptr_lines[4 * mesh_curved + 2] = m_index_vertices + 1 * (mesh_curved + 1) + mesh_curved;
	ibo_ptr_lines[4 * mesh_curved + 3] = m_index_vertices + 1 * (mesh_curved + 1) + mesh_curved;
	//update
	m_index_lines += 4 * (mesh_curved + 1);
	m_index_vertices += 4 * (mesh_curved + 1);
	m_index_triangles += 6 * (mesh_curved + 0);
}
void Draw::update_fiber_rectangle(const sections::fibers::Rectangle* fiber)
{
	//data
	const double s2 = fiber->width();
	const double s3 = fiber->height();
	const double x2 = fiber->position(0);
	const double x3 = fiber->position(1);
	uint32_t* ibo_ptr_lines = m_ibo.data() + m_counter_points + m_index_lines;
	uint32_t* ibo_ptr_triangles = m_ibo.data() + m_counter_points + m_counter_lines + m_index_triangles;
	canvas::vertices::Model3D* vbo_ptr = (canvas::vertices::Model3D*) m_vbo.data() + m_index_vertices;
	//vbo data
	for(uint32_t i = 0; i < 8; i++) vbo_ptr[i].m_position[2] = 0;
	for(uint32_t i = 0; i < 4; i++) vbo_ptr[i + 0].m_color = "white";
	for(uint32_t i = 0; i < 4; i++) vbo_ptr[i + 4].m_color = fiber_color(fiber);
	vbo_ptr[0].m_position[0] = vbo_ptr[3].m_position[0] = vbo_ptr[4].m_position[0] = vbo_ptr[7].m_position[0] = -x3 + s3 / 2;
	vbo_ptr[1].m_position[0] = vbo_ptr[2].m_position[0] = vbo_ptr[5].m_position[0] = vbo_ptr[6].m_position[0] = -x3 - s3 / 2;
	vbo_ptr[0].m_position[1] = vbo_ptr[1].m_position[1] = vbo_ptr[4].m_position[1] = vbo_ptr[5].m_position[1] = +x2 - s2 / 2;
	vbo_ptr[2].m_position[1] = vbo_ptr[3].m_position[1] = vbo_ptr[6].m_position[1] = vbo_ptr[7].m_position[1] = +x2 + s2 / 2;
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
	m_index_lines += 8;
	m_index_vertices += 8;
	m_index_triangles += 6;
}

//color
canvas::Color Draw::node_color(const sections::Node* node) const
{
	//data
	const Type tw[] = {Type::MeshWarpingT, Type::MeshWarpingS2, Type::MeshWarpingS3};
	const canvas::palettes::Palette& palette = ((canvas::objects::Palette*) m_scene->object(1))->palette();
	//color
	for(uint32_t i = 0; i < 3; i++)
	{
		if(m_type == tw[i])
		{
			return palette.color(node->warping(i), m_ue[i + 0], m_ue[i + 3]);
		}
	}
	//return
	return "blue";
}
canvas::Color Draw::fiber_color(const sections::fibers::Fiber* fiber) const
{
	//data
	const Type tw[] = {
		Type::FibersWarpingT, Type::FibersWarpingS2, Type::FibersWarpingS3
	};
	const Type tg[] = {
		Type::FibersGradientT2, Type::FibersGradientS22, Type::FibersGradientS32, Type::FibersGradientT3, Type::FibersGradientS23, Type::FibersGradientS33
	};
	const canvas::palettes::Palette& palette = ((canvas::objects::Palette*) m_scene->object(1))->palette();
	//warping
	for(uint32_t i = 0; i < 3; i++)
	{
		if(m_type == tw[i])
		{
			return palette.color(fiber->warping(i), m_ue[i + 0], m_ue[i + 3]);
		}
	}
	//gradient
	for(uint32_t i = 0; i < 6; i++)
	{
		if(m_type == tg[i])
		{
			return palette.color(fiber->gradient(i % 3, i / 3), m_ge[i + 0], m_ge[i + 6]);
		}
	}
	//return
	return "blue";
}