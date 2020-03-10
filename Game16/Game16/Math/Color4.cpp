#include "Color4.h"
#include <cmath>

//デフォルトコンストラクタ
Color4::Color4()
	:r(0),
	g(0),
	b(0),
	a(1)
{
}

Color4::Color4(float R, float G, float B, float A)
	:r(R),
	g(G),
	b(B),
	a(A)
{
}

Color4 & Color4::saturate()
{
	// TODO: return ステートメントをここに挿入します
	
	r = fmin(1.0, fmax(0.0, r));
	g = fmin(1.0, fmax(0.0, g));
	b = fmin(1.0, fmax(0.0, b));
	a = fmin(1.0, fmax(0.0, a));
	return *this;
}

Color4 Color4::operator+() const
{
	return *this;
}

Color4 Color4::operator-() const
{
	return Color4(1 - r, 1 - g, 1 - b, a);
}

Color4 & Color4::operator+=(const Color4 & c2)
{
	// TODO: return ステートメントをここに挿入します
	r += c2.r;
	g += c2.g;
	b += c2.b;
	a += c2.a;
	return *this;
}

Color4 & Color4::operator-=(const Color4 & c2)
{
	// TODO: return ステートメントをここに挿入します
	r -= c2.r;
	g -= c2.g;
	b -= c2.b;
	a -= c2.a;
	return *this;
}

Color4 & Color4::operator*=(const Color4 & c2)
{
	// TODO: return ステートメントをここに挿入します
	r *= c2.r;
	g *= c2.g;
	b *= c2.b;
	a *= c2.a;
	return *this;
}


Color4 & Color4::operator*=(float s)
{
	// TODO: return ステートメントをここに挿入します
	r *= s;
	g *= s;
	b *= s;
	a *= s;
	return *this;
}

Color4 & Color4::operator/=(float s)
{
	// TODO: return ステートメントをここに挿入します
	if (s == 0)
	{
		r = r;
		g = g;
		b = b;
		a = a;
	}
	else
	{
		r /= s;
		g /= s;
		b /= s;
		a /= s;
	}
	return *this;
}

const Color4 operator+(const Color4 & c1, const Color4 & c2)
{
	return Color4(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a);
}

const Color4 operator-(const Color4 & c1, const Color4 & c2)
{
	return Color4(c1.r- c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a);
}

const Color4 operator*(const Color4 & c1, const Color4 & c2)
{
	return Color4(c1.r* c2.r, c1.g * c2.g, c1.b * c2.b, c1.a * c2.a);
}

const Color4 operator*(float s, const Color4 & c)
{
	return Color4(c.r*s, c.g*s, c.b*s, c.a*s);
}

const Color4 operator*(const Color4 & c, float s)
{
	return Color4(c.r*s, c.g*s, c.b*s, c.a*s);
}

const Color4 operator/(const Color4 & c, float s)
{
	return Color4(c.r / s, c.g / s, c.b / s, c.a / s);
}
