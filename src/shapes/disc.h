#ifndef DISC_H_INCLUDED
#define DISC_H_INCLUDED

#include "shape.h"

#include "vec3.h"
#include "point.h"
#include "ray.h"
#include "rgb.h"

class Disc : public Shape
{
	public:
		Disc(Point a_center, Vec3 a_normal, double a_radius, Material *a_material)
		{
            center      = a_center;
			normal 	    = a_normal;
			radius      = a_radius;
			material	= a_material;

			trans 	    = new Transform;

			shadow		= true;
			bounds		= false;
		}
		Disc()
		{
		    center      = Point(0.0f);
			normal 	    = Vec3(0.0f, 1.0f, 0.0f);
			radius     	= 1.0f;
			material	= nullptr;

			trans   	= new Transform;

			shadow		= true;
			bounds		= false;
		}
		~Disc() { if(trans != nullptr) delete trans; }

		bool hit(const Ray &r, double min_dist, double max_dist, HitRecord &hit_r) const;
		bool shadow_hit(const Ray &r, double min_dist, double max_dist) const;
		bool get_random_point(const Point &view_pos, CRandomMersenne *rng, Point &light_pos) const;

	//private:
        Point   center;
        Vec3 	normal;
        double  radius;

    private:
        static const double kdisc_epsilon;

};

#endif // DISC_H_INCLUDED
