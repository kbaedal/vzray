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
		Plane(Vec3 a_normal, double a_dist, Material *a_material)
		{
			normal 	    = a_normal;
			dist	 	= a_dist;
			material	= a_material;

			trans 	    = new Transform;

			shadow		= true;
			bounds		= false;
		}
		Plane()
		{
			normal 	    = Vec3(0.0f, 1.0f, 0.0f);
			dist     	= 0.0f;
			material	= nullptr;

			trans   	= new Transform;

			shadow		= true;
			bounds		= false;
		}
		~Plane() { if(trans != nullptr) delete trans; }

		bool hit(const Ray &r, double min_dist, double max_dist, HitRecord &hit) const;
		bool shadow_hit(const Ray &r, double min_dist, double max_dist) const;
		bool get_random_point(const Point &view_pos, CRandomMersenne *rng, Point &light_pos) const;

	//private:
		Vec3 	normal;
		double 	dist;

    private:
        static const double kplane_epsilon;

};

#endif // __PLANE_H__
