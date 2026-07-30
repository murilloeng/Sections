#pragma once

//Sections
#include "Sections/inc/Profile.hpp"

namespace sections
{
	class ProfileI : public Profile
	{
	public:
		//constructor
		ProfileI(void);
		ProfileI(const char*);

		//destructor
		~ProfileI(void);

		//data
		void catalog(const char*);

	private:
		//geometry
		void setup_geometry_1(void) const;
		void setup_geometry_2(void) const;

		void setup_fibers(void) override;
		void setup_isolated(void) override;
		void setup_geometry(void) const override;
	};
}