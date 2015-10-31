#ifndef __PARALLELOGRAM_H__
#define __PARALLELOGRAM_H__ 1

#include "shape.h"

#include "vec3.h"
#include "point.h"
#include "ray.h"
#include "rgb.h"


class Parallelogram : public Shape
{
	public:
		Parallelogram(Point a_base, Vec3 a_u, Vec3 a_v, Material *a_material)
		{
			base	    = a_base;
			u		    = a_u;
			v		    = a_v;
			material    = a_material;

			normal 	= versor(cross(u, v));

			u_normal = versor(u);
			v_normal = versor(v);

			trans = new Transform;
		}
		~Parallelogram() { if(trans != NULL) delete trans; }

		bool hit(const Ray &r, float min_dist, float max_dist, HitRecord &hit) const;
		bool shadow_hit(const Ray &r, float min_dist, float max_dist) const;
		bool get_random_point(const Point &view_pos, CRandomMersenne *rng, Point &light_pos) const;

	//private:
		Point 	base;

		Vec3 	u,
				v,
				normal,
				u_normal,
				v_normal;
    private:
        static const float kparall_epsilon;
};

#endif // __PARALLELOGRAM_H__
