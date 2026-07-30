#pragma once

namespace sections
{
	namespace catalog
	{
		struct CHS
		{
			char m_label[20];
			static CHS m_list[215];
			double m_diameter, m_thickness;
		};

	}
}