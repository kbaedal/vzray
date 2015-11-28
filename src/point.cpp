#include "point.h"
#include "vec3.h"

#include "randomc/randomc.h"
#include "onb.h"

Point::Point(const Vec3 &v)
{
    e[0] = v.x();
    e[1] = v.y();
    e[2] = v.z();
}

Point &Point::operator+=(Point const &p)
{
    e[0] += p.e[0];
    e[1] += p.e[1];
    e[2] += p.e[2];

    return *this;
}

std::ostream &operator<<(std::ostream &os, const Point &p)
{
	os << "Point<" << p.e[0] << ", " << p.e[1] << ", " << p.e[2] << ">";

	return os;
}

bool operator==(const Point &p1, const Point &p2)
{
	return ((p1.e[0] == p2.e[0]) && (p1.e[1] == p2.e[1]) && (p1.e[2] == p2.e[2]));
}

bool operator!=(const Point &p1, const Point &p2)
{
	return !(p1 == p2);
}

Point operator+(Point p1, const Point &p2)
{
	return Point(p1.x() + p2.x(), p1.y() + p2.y(), p1.z() + p2.z());
}

Point operator-(Point p1, const Point &p2)
{
	return Point(p1.x() - p2.x(), p1.y() - p2.y(), p1.z() - p2.z());
}

Point operator*(Point p, double f)
{
	return Point(p.x() * f, p.y() * f, p.z() * f);
}

Point operator*(double f, Point p)
{
	return Point(p.x() * f, p.y() * f, p.z() * f);
}

Point operator/(Point p, double f)
{
    double inv = 1.0f / f;

	return Point(p.x() * inv, p.y() * inv, p.z() * inv);
}

void Point::set(const Vec3 &v)
{
	e[0] = v.x();
	e[1] = v.y();
	e[2] = v.z();
}
