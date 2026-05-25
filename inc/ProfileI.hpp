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

		//destructor
		~ProfileI(void);

	private:
		//geometry
		void setup_geometry_1(void) const;
		void setup_geometry_2(void) const;
		void setup_geometry(void) const override;
	};
}