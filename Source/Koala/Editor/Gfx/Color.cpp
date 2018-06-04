#include <Koala/Editor/Gfx/Color.h>
#include <utility>
#include <algorithm>

namespace Koala::Editor::Gfx {

Color::Color(float red, float green, float blue, float alpha)
{
	m_RGBA[0] = red;
	m_RGBA[1] = green;
	m_RGBA[2] = blue;
	m_RGBA[3] = alpha;
}

void Color::SetRed(float value)
{
	m_RGBA[0] = std::clamp(value, 0.0f, 1.0f);
}
void Color::SetGreen(float value)
{
	m_RGBA[1] = std::clamp(value, 0.0f, 1.0f);
}
void Color::SetBlue(float value)
{
	m_RGBA[2] = std::clamp(value, 0.0f, 1.0f);
}
void Color::SetAlpha(float value)
{
	m_RGBA[3] = std::clamp(value, 0.0f, 1.0f);
}
float Color::GetRed() const
{
	return m_RGBA[0];
}
float Color::GetGreen() const
{
	return m_RGBA[1];
}
float Color::GetBlue() const
{
	return m_RGBA[2];
}
float Color::GetAlpha() const
{
	return m_RGBA[3];
}

void Color::Clamp(float range0, float range1)
{
	// Make range0 lower point
	if(range1 < range0)
	{
		std::swap(range0, range1);
	}
	range0 = std::clamp(range0, 0.0f, 1.0f);
	range1 = std::clamp(range1, 0.0f, 1.0f);

	for( auto& color : m_RGBA )
	{
		color = std::clamp(color, range0, range1);
	}
}

} // namespace Koala::Editor::Gfx
