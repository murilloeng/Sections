//std
#include <cstdio>
#include <cstdlib>

//Sections
#include "Sections/inc/RHS.hpp"
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
		sections::Rectangle section;
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