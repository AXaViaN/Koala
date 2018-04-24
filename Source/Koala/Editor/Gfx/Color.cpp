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
	std::clamp(value, 0.0f, 1.0f);
	m_RGBA[0] = value;
}
void Color::SetGreen(float value)
{
	std::clamp(value, 0.0f, 1.0f);
	m_RGBA[1] = value;
}
void Color::SetBlue(float value)
{
	std::clamp(value, 0.0f, 1.0f);
	m_RGBA[2] = value;
}
void Color::SetAlpha(float value)
{
	std::clamp(value, 0.0f, 1.0f);
	m_RGBA[3] = value;
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
	std::clamp(range0, 0.0f, 1.0f);
	std::clamp(range1, 0.0f, 1.0f);

	for( auto& color : m_RGBA )
	{
		std::clamp(color, range0, range1);
	}
}

} // namespace Koala::Editor::Gfx
