#pragma once

//Sections
#include "Sections/inc/Profile.hpp"

namespace sections
{
	class ProfileT : public Profile
	{
	public:
		//constructor
		ProfileT(void);

		//destructor
		~ProfileT(void);

	private:
		//geometry
		void setup_geometry_1(void) const;
		void setup_geometry_2(void) const;

		void setup_isolated(void) override;
		void setup_geometry(void) const override;
	};
}