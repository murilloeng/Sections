//std
#include <cstdio>
#include <cstdlib>

//Sections
#include "Sections/inc/Rectangle.hpp"

int main(void)
{
	try
	{
		//data
		sections::Rectangle section;
		//setup
		section.width(0.20);
		section.height(0.60);
		//compute
		section.compute();
	}
	catch(const std::exception& exception)
	{
		printf("%s\n", exception.what());
	}
	//return
	return EXIT_SUCCESS;
}