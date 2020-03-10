#include "Vector3.h"
#include <cmath>
using namespace DirectX;

//デフォルトコンストラクタ
Vector3::Vector3()
	:x(0),y(0),z(0)
{
}
//コンストラクタ
Vector3::Vector3(float X, float Y, float Z)
	:x(X),y(Y),z(Z)
{
}
DirectX::XMVECTOR Vector3::toXMVector()
{
	XMVECTOR xmVector = XMVECTOR();
	xmVector.m128_f32[0] = x;
	xmVector.m128_f32[1] = y;
	xmVector.m128_f32[2] = z;
	return xmVector;
}
//長さ
float Vector3::length() const
{
	return (float)std::sqrtf(x * x + y * y + z * z);
}
//正規化
Vector3 & Vector3::normalize()
{
	float len;
	if ((len = length()) != 0)
		*this /= len;

	return *this;
}

float Vector3::dot(const Vector3 & v) const
{
	return x * v.x + y * v.y + z * v.z;
	 
}

Vector3 Vector3::cross(const Vector3 & v) const
{
	return Vector3((y * v.z - v.y* z), (-1)*(x*v.z - v.x * z), (x*v.y - v.x *y));
}

Vector3 Vector3::operator+() const
{
	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3 & Vector3::operator+=(const Vector3 & v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3 & Vector3::operator-=(const Vector3 & v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3 & Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3 & Vector3::operator/=(float s)
{

	if (s == 0)
	{
		x = x;
		y = y;
		z = z;
	}
	else
	{
		x /= s;
		y /= s;
		z /= s;
	}
	
	return *this;
}

const Vector3 operator+(const Vector3 & v1, const Vector3 & v2)
{
	return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

const Vector3 operator-(const Vector3 & v1, const Vector3 & v2)
{
	return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

const Vector3 operator*(const Vector3 & v, float s)
{
	return Vector3(v.x*s, v.y*s, v.z*s);
}

const Vector3 operator*(float s, const Vector3 & v)
{
	return Vector3(v.x*s, v.y*s, v.z* s);
}

const Vector3 operator/(const Vector3 & v, float s)
{
	return Vector3(v.x / s, v.y / s, v.z / s);
}
