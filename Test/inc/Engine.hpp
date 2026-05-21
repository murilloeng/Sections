#pragma once

//glfw
#include <GLFW/glfw3.h>

//Sections
#include "Sections/inc/Section.hpp"
#include "Sections/Test/inc/Draw.hpp"

//Canvas
#include "Canvas/inc/Scene/Scene.hpp"
#include "Canvas/inc/Cameras/Click.hpp"

class Engine
{
public:
	//constructor
	Engine(void);

	//destructor
	~Engine(void);

	//start
	void start(void);

	//data
	bool show_fps(bool);
	bool show_fps(void) const;

	double framerate(double);
	double framerate(void) const;

	sections::Section* section(void);
	sections::Section* section(sections::Section*);

private:
	//setup
	void setup_glfw(void);
	void setup_scene(void);
	void setup_callbacks(void);

	//update
	static void update_nodes(GLFWwindow*);
	static void update_playing(GLFWwindow*);
	static void update_elements(GLFWwindow*);
	static void update_step(GLFWwindow*, bool);
	static void update_framerate(GLFWwindow*, bool);

	//canvas
	static uint32_t canvas_modifiers(int32_t);
	static canvas::button canvas_button(int32_t);

	//callbacks
	static void callback_wheel(GLFWwindow*, double, double);
	static void callback_button(GLFWwindow*, int, int, int);
	static void callback_resize(GLFWwindow*, int32_t, int32_t);
	static void callback_position(GLFWwindow*, double, double);
	static void callback_keyboard(GLFWwindow*, int32_t, int32_t, int32_t, int32_t);

	//data
	Draw* m_draw;
	bool m_playing;
	bool m_show_fps;
	double m_framerate;
	GLFWwindow* m_window;
	canvas::Scene* m_scene;
	sections::Section* m_section;
};