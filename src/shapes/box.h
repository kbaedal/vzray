#ifndef __BOX_HPP__
#define __BOX_HPP__ 1

#include <ctime>

#include "vec3.h"
#include "point.h"
#include "ray.h"
#include "rgb.h"
#include "shapes/shape.h"
#include "randomc/randomc.h"

class Box : public Shape
{
	public:
		Box(const Point &a_minimo, const Point &a_maximo, Material *a_material);
		Box()
		{
			maximo.set( 1.0f,  1.0f,  1.0f);
			minimo.set(-1.0f, -1.0f, -1.0f);

			material = nullptr;

			trans = new Transform;

			shadow = true;
			bounds = true;
		};
		~Box() { std::cerr << "\n\nBOX DESTRUCTOR.\n\n" << std::flush; if(trans != nullptr) delete trans; };

		bool hit(const Ray &r, float min_dist, float max_dist, HitRecord &hit) const;
		bool shadow_hit(const Ray &r, float min_dist, float max_dist) const;
		bool get_random_point(const Point &view_pos, CRandomMersenne *rng, Point &light_pos) const;

	//private:
		Point	minimo, maximo;
};

#endif // __BOX_HPP__
