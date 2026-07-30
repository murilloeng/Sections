#pragma once

namespace sections
{
	namespace catalog
	{
		struct RHS
		{
			char m_label[20];
			static RHS m_list[218];
			double m_height, m_width, m_thickness, m_radius_outer, m_radius_inner;
		};
	}
}