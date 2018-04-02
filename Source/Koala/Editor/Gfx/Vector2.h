#ifndef KOALA__EDITOR__GFX__VECTOR2
#define KOALA__EDITOR__GFX__VECTOR2

#include <Koala/Editor/Gfx/Color.h>

namespace Koala::Editor::Gfx {

class Vector2
{
public:
	Vector2()
	{ }
	Vector2(float x, float y) : 
		m_X(x),
		m_Y(y)
	{ }

	void SetX(float x)
	{
		m_X = x;
	}
	void SetY(float y)
	{
		m_Y = y;
	}
	float GetX() const
	{
		return m_X;
	}
	float GetY() const
	{
		return m_Y;
	}

	Vector2 operator+(const Vector2& other) const
	{
		return Vector2(this->m_X + other.m_X,
					   this->m_Y + other.m_Y);
	}
	Vector2 operator-(const Vector2& other) const
	{
		return Vector2(this->m_X - other.m_X,
					   this->m_Y - other.m_Y);
	}
	Vector2& operator+=(const Vector2& other)
	{
		*this = *this + other;
		return *this;
	}
	Vector2& operator-=(const Vector2& other)
	{
		*this = *this - other;
		return *this;
	}

private:
	float m_X = 0.0f;
	float m_Y = 0.0f;

};

} // namespace Koala::Editor::Gfx

#endif // KOALA__EDITOR__GFX__VECTOR2
