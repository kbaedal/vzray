#include "aabb/aabb.h"

#include "vec3.h"
#include "point.h"
#include "ray.h"

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

bool AABB::hit(const Ray &r, double min_dist, double max_dist) const
{
	double t1 = (minimo.x() - r.origin().x()) * r.inv_dir().x();
	double t2 = (maximo.x() - r.origin().x()) * r.inv_dir().x();
	double t3 = (minimo.y() - r.origin().y()) * r.inv_dir().y();
	double t4 = (maximo.y() - r.origin().y()) * r.inv_dir().y();
	double t5 = (minimo.z() - r.origin().z()) * r.inv_dir().z();
	double t6 = (maximo.z() - r.origin().z()) * r.inv_dir().z();

	double tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	double tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	// If tmax < 0, ray (line) is intersecting the box, but the whole box is behind us
	if (tmax < 0.0f)
		return false;

	// If tmin > tmax, ray doesn't intersect box.
	if (tmin > tmax)
		return false;

	// Hit out of range.
	if((tmin < min_dist) || (tmin > max_dist))
		return false;

    // Ray intersects the box.
	return true;
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
