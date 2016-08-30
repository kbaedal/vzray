#include "shapes/isecaux.h"
#include "shapes/triangle.h"

bool Triangle::hit(const Ray &r, double min_dist, double max_dist, HitRecord &hit) const
{
	double 	dist;
	Vec3	dir10, dir20, temp_normal;

	if(isecaux::test_ray_triangle(r, p0, p1, p2, min_dist, max_dist, dist))	{
		dir10 = p1 - p0;
		dir20 = p2 - p0;

		temp_normal = cross(dir10, dir20);
		temp_normal.normalize();

		hit.dist 	    = dist;
		hit.normal      = temp_normal;
		hit.material    = material;

		return true;
	}

	return false;
}

bool Triangle::shadow_hit(const Ray &r, double min_dist, double max_dist) const
{
	double dist;

	return isecaux::test_ray_triangle(r, p0, p1, p2, min_dist, max_dist, dist);
}
