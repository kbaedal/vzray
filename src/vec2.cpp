#include "vec2.h"

bool operator==(const Vec2 &v1, const Vec2 &v2)
{
	return ((v1.e[0] == v2.e[0]) && (v1.e[1] == v2.e[1]));
}

bool operator!=(const Vec2 &v1, const Vec2 &v2)
{
	return !(v1 == v2);
}

Vec2 operator+(const Vec2 &v1, const Vec2 &v2)
{
	return Vec2(v1.x() + v2.x(), v1.y() + v2.y());
}

Vec2 operator-(const Vec2 &v1, const Vec2 &v2)
{
	return Vec2(v1.x() - v2.x(), v1.y() - v2.y());
}

Vec2 operator*(const Vec2 &v, double f)
{
	return Vec2(v.x() * f, v.y() * f);
}

Vec2 operator*(double f, const Vec2 &v)
{
	return Vec2(v.x() * f, v.y() * f);
}

Vec2 operator/(const Vec2 &v, double f)
{
	return Vec2(v.x() / f, v.y() / f);
}

Vec2 operator/(double f, const Vec2 &v)
{
	return Vec2(v.x() / f, v.y() / f);
}

Vec2 versor(Vec2 const &v)
{
	Vec2 t(v);
	return t / t.length();
}

std::ostream& operator<<(std::ostream &os, const Vec2 &v)
{
	os << '<' << v.e[0] << ", " << v.e[1] << '>';

	return os;
}

