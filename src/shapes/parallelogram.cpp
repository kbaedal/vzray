#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

#include <float.h>

#include "parallelogram.h"

#include "onb.h"

const double Parallelogram::kparall_epsilon = 1e-8;

bool Parallelogram::hit(const Ray &r, double min_dist, double max_dist, HitRecord &hit) const
{
	Point 	hit_plane;
	Vec3	offset;
	float	dot1,
            dot2,
            t,
            u1,
            v1;

	dot1 = dot(r.direction(), normal);

	if(dot1 < kparall_epsilon && dot1 > -kparall_epsilon) return false;

	dot2 = dot(normal, Vec3(base));
	t = (dot2 - dot(normal, r.origin())) / dot1;

	if(t > max_dist || t < min_dist) return false;

	hit_plane = r.get_point(t);
	offset.set(hit_plane - base);

	u1 = dot(u_normal, offset) / u.length();
	if(u1 < 0.0f || u1 > 1.0f) return false;

	v1 = dot(v_normal, offset) / v.length();
	if(v1 < 0.0f || v1 > 1.0f) return false;

	hit.dist 	    = t;
	hit.normal      = versor(normal);
	hit.material	= material;

	return true;
}

bool Parallelogram::shadow_hit(const Ray &r, double min_dist, double max_dist) const
{
	Point 	hit_plane;
	Vec3	offset;
	float	dot1,
            dot2,
            t,
            u1,
            v1;

	dot1 = dot(r.direction(), normal);

	if(dot1 < kparall_epsilon && dot1 > -kparall_epsilon) return false;

	dot2 = dot(normal, Vec3(base));
	t = (dot2 - dot(normal, r.origin())) / dot1;

	if(t > max_dist || t < min_dist) return false;

	hit_plane = r.get_point(t);
	offset.set(hit_plane - base);

	u1 = dot(u_normal, offset) / u.length();
	if(u1 < 0.0f || u1 > 1.0f) return false;

	v1 = dot(v_normal, offset) / v.length();
	if(v1 < 0.0f || v1 > 1.0f) return false;

	return true;
}

bool Parallelogram::get_random_point(const Point &view_pos, CRandomMersenne *rng, Point &light_pos) const
{
	light_pos = Point(base + (Point(u) * rng->Random()) + (Point(v) * rng->Random()));
	return true;
}
