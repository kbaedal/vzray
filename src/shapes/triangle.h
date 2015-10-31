#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__ 1

#include "vec3.h"
#include "ray.h"
#include "rgb.h"
#include "shapes/shape.h"

class Triangle : public Shape
{
	public:
		Triangle(const Point &a_p0, const Point &a_p1, const Point &a_p2, Material *a_material)
		{
			p0 			= a_p0;
			p1 			= a_p1;
			p2 			= a_p2;
			material	= a_material;
			trans       = new Transform;
		}
		Triangle()
		{
			p0.set(0.0f, 0.0f, 0.0f);
			p1.set(1.0f, 0.0f, 0.0f);
			p2.set(0.0f, 0.0f, 1.0f);
			material    = nullptr;
			trans       = new Transform;
		}

		bool hit(const Ray &r, float min_dist, float max_dist, HitRecord &hit) const;
		bool shadow_hit(const Ray &r, float min_dist, float max_dist) const;
		bool get_random_point(const Point &view_pos, CRandomMersenne *rng, Point &light_pos) const
		{
			light_pos = Point(0.0f);
			return true;
		}

		Point 	p0, p1, p2;
};

#endif // __TRIANGLE_H__
