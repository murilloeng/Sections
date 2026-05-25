//std
#include <cstdio>
#include <cstdlib>

//Sections
#include "Sections/inc/RHS.hpp"
#include "Sections/inc/ProfileI.hpp"
#include "Sections/inc/ProfileT.hpp"
#include "Sections/inc/Rectangle.hpp"
#include "Sections/Test/inc/Engine.hpp"

//draw
void draw(sections::Section* section)
{
	//data
	Engine engine;
	engine.show_fps(false);
	engine.section(section);
	//start
	engine.start();
}

int main(void)
{
	try
	{
		//data
		sections::RHS section;
		section.mesh_size(1.30e-03);
		section.radius_inner(2.60e-03);
		section.radius_outer(3.90e-03);
		//compute
		section.compute();
		//print
		section.print();
		//draw
		draw(&section);
	}
	catch(const std::exception& exception)
	{
		printf("%s\n", exception.what());
	}
	//return
	return EXIT_SUCCESS;
}