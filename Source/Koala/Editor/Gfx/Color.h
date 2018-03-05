#ifndef KOALA__EDITOR__GFX__COLOR
#define KOALA__EDITOR__GFX__COLOR

namespace Koala::Editor::Gfx {

struct Color
{
	float& r;
	float& g;
	float& b;
	float& a;

public:
	Color() : 
		Color(0.0f, 0.0f, 0.0f, 1.0f)
	{ }
	Color(float red, float green, float blue) : 
		Color(red, green, blue, 1.0f)
	{ }
	Color(float red, float green, float blue, float alpha);

	void Clamp();
	void Clamp(float range0, float range1);

private:
	float m_RGBA[4];

};

} // namespace Koala::Editor::Gfx

#endif // KOALA__EDITOR__GFX__COLOR
