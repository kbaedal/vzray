#include "aabb/aabb.h"

void AABB::set(const Point &a_min, const Point &a_max)
{
	minimo.e[0] = std::min(a_min.x(), a_max.x());
	minimo.e[1] = std::min(a_min.y(), a_max.y());
	minimo.e[2] = std::min(a_min.z(), a_max.z());

	maximo.e[0] = std::max(a_min.x(), a_max.x());
	maximo.e[1] = std::max(a_min.y(), a_max.y());
	maximo.e[2] = std::max(a_min.z(), a_max.z());
}

bool AABB::overlaps(const AABB &b)
{
	bool x, y, z;

	x = (maximo.x() >= b.minimo.x()) && (minimo.x() <= b.maximo.x());
	y = (maximo.y() >= b.minimo.y()) && (minimo.y() <= b.maximo.y());
	z = (maximo.z() >= b.minimo.z()) && (minimo.z() <= b.maximo.z());

	return (x && y && z);
}


bool AABB::inside(const Point &p)
{
	if((p.x() >= minimo.x()) && (p.x() <= maximo.x()))
		if((p.y() >= minimo.y()) && (p.y() <= maximo.y()))
			if((p.z() >= minimo.z()) && (p.z() <= maximo.z()))
				return true;

	return false;
}

AABB surround(const AABB &b1, const AABB &b2)
{
	AABB temp;

	temp.minimo.e[0] = std::min(b1.minimo.x(), b2.minimo.x());
	temp.minimo.e[1] = std::min(b1.minimo.y(), b2.minimo.y());
	temp.minimo.e[2] = std::min(b1.minimo.z(), b2.minimo.z());

	temp.maximo.e[0] = std::max(b1.maximo.x(), b2.maximo.x());
	temp.maximo.e[1] = std::max(b1.maximo.y(), b2.maximo.y());
	temp.maximo.e[2] = std::max(b1.maximo.z(), b2.maximo.z());

	return temp;
}

AABB surround(const AABB &b, const Point &p)
{
	AABB temp;

	temp.minimo.e[0] = std::min(b.minimo.x(), p.x());
	temp.minimo.e[1] = std::min(b.minimo.y(), p.y());
	temp.minimo.e[2] = std::min(b.minimo.z(), p.z());

	temp.maximo.e[0] = std::max(b.maximo.x(), p.x());
	temp.maximo.e[1] = std::max(b.maximo.y(), p.y());
	temp.maximo.e[2] = std::max(b.maximo.z(), p.z());

	return temp;
}
