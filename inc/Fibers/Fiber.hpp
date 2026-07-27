#pragma once

//std
#include <vector>
#include <cstdint>

namespace sections
{
	class Section;
}

namespace sections
{
	namespace fibers
	{
		class Fiber
		{
		public:
			//constructor
			Fiber(void);

			//destructor
			virtual ~Fiber(void);

			//data
			double area(void) const;

			double warping(uint32_t) const;
			const double* warping(void) const;

			double position(uint32_t) const;
			const double* position(void) const;

			const double* gradient(void) const;
			double gradient(uint32_t, uint32_t) const;

			//create
			static void create_rectangles(std::vector<Fiber*>&, double, double, double, double, uint32_t, uint32_t);

		protected:
			//analysis
			void setup(Section*);

			//data
			double m_area;
			double m_warping[3];
			double m_position[2];
			double m_gradient[6];
			double m_orientation;
			double* m_residual_stress;
		};
	}
}