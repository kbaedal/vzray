#ifndef __PLANE_H__
#define __PLANE_H__ 1

#include "shape.h"

#include "vec3.h"
#include "point.h"
#include "ray.h"
#include "rgb.h"

class Plane : public Shape
{
	public:
		Plane(Point a_base, Vec3 a_normal, Material *a_material)
		{
			base        = a_base;
			normal 	    = versor(a_normal);
			material	= a_material;

			trans 	    = new Transform;

			shadow		= true;
			bounds		= false;
		}
		Plane()
		{
		    base        = Point(0.0f);
			normal 	    = Vec3(0.0f, 1.0f, 0.0f);
			material	= nullptr;

			trans   	= new Transform;

			shadow		= true;
			bounds		= false;
		}
		~Plane() { if(trans != nullptr) delete trans; }

		bool hit(const Ray &r, double min_dist, double max_dist, HitRecord &hit_r) const;
		bool shadow_hit(const Ray &r, double min_dist, double max_dist) const;
		bool get_random_point(const Point &view_pos, CRandomMersenne *rng, Point &light_pos) const;

	private:
        Point   base;
		Vec3 	normal;
};

#endif // __PLANE_H__
