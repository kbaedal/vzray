#ifndef __CYLINDER_HPP__
#define __CYLYNDER_HPP__ 1

#include "shape.h"

#include "vec3.h"
#include "point.h"
#include "ray.h"
#include "rgb.h"
#include "randomc/randomc.h"

class Cylinder : public Shape
{
	public:
		Cylinder(Point a_bottom, Point a_top, double a_radius, Material *a_material);
		Cylinder()
		{
			bottom 		= Point( 0.0f, 0.0f, -1.0f);
			top 		= Point( 0.0f, 0.0f,  1.0f);
			radius  	= 1.0f;
			material	= nullptr;
			trans 	    = new Transform;
			shadow      = true;
			bounds		= true;
		}
		~Cylinder() { if(trans != NULL) delete trans; }

		bool hit(const Ray &r, double min_dist, double max_dist, HitRecord &hit) const;
		bool shadow_hit(const Ray &r, double min_dist, double max_dist) const;
		bool get_random_point(const Point &view_pos, CRandomMersenne *rng, Point &light_pos) const;

	//private:
		Point 			bottom,
                        top;
		double 			radius;

    private:
        static const double kpi;
};

#endif // __CYLINDER_HPP__

