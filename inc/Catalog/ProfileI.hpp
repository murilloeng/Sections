#pragma once

namespace sections
{
	namespace catalog
	{
		struct ProfileI
		{
			char m_label[20];
			static ProfileI m_list[292];
			double m_height, m_width, m_web_thickness, m_flange_thickness, m_radius;
		};
	}
}