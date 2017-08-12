#ifndef __SPHERE_H__
#define __SPHERE_H__ 1

#include "shape.h"

#include "vec3.h"
#include "ray.h"
#include "rgb.h"

#include "randomc/randomc.h"

class Sphere : public Shape
{
	public:
		Sphere(Point a_center, double a_radius, Material *a_material);
		Sphere()
		{
			center 	    = Point(0.0f);
			radius 	    = 1.0f;
			material	= nullptr;
			trans 	    = new Transform;
			shadow 	    = true;
			bounds		= true;
		}
		~Sphere() { if(trans != nullptr) delete trans; }

		bool hit(const Ray &r, double min_dist, double max_dist, HitRecord &hit) const;
		bool shadow_hit(const Ray &r, double min_dist, double max_dist) const;
		bool get_random_point(const Point &view_pos, CRandomMersenne *rng, Point &light_pos) const;
	//private:
		Point			center;
		double 			radius;
};

#endif // __SPHERE_HPP__
