#include <Koala/Editor/Gfx/Color.h>
#include <utility>

namespace Koala::Editor::Gfx {

Color::Color(float red, float green, float blue, float alpha) : 
	r(m_RGBA[0]), 
	g(m_RGBA[1]), 
	b(m_RGBA[2]), 
	a(m_RGBA[3])
{
	m_RGBA[0] = red;
	m_RGBA[1] = green;
	m_RGBA[2] = blue;
	m_RGBA[3] = alpha;
}

void Color::Clamp()
{
	constexpr float DefaultRange0 = 0.0f;
	constexpr float DefaultRange1 = 1.0f;
	Clamp(DefaultRange0, DefaultRange1);
}
void Color::Clamp(float range0, float range1)
{
	// Make range0 lower point
	if(range1 < range0)
	{
		float tmp = std::move(range0);
		std::swap(range0, range1);
	}

	for( auto& color : m_RGBA )
	{
		if(color < range0)
		{
			color = range0;
		}
		else if(color > range1)
		{
			color = range1;
		}
	}
}

} // namespace Koala::Editor::Gfx
