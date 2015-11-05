#ifndef __SHAPE_H__
#define __SHAPE_H__ 1

#include "vec3.h"
#include "point.h"
#include "ray.h"
#include "rgb.h"
#include "material/material.h"
#include "matrix/transform.h"
#include "randomc/randomc.h"
#include "aabb/aabb.h"

class Shape
{
	public:
		virtual ~Shape() {};

		virtual bool hit(const Ray &r, double min_dist, double max_dist, HitRecord &hit) const = 0;
		virtual bool shadow_hit(const Ray &r, double min_dist, double max_dist) const = 0;
		virtual bool get_random_point(const Point &view_pos, CRandomMersenne *rng, Point &light_pos) const = 0;

		Material *get_material() { return material; }
		void set_material(Material *new_material) { material = new_material; }
		Transform *get_transform() { return trans; }
		void set_trans(const Transform *new_trans)
		{
			// Update transform
			trans->update(*new_trans);

			// Update AABB
			Transform t;

			t.update(*new_trans);
			aabb = t.update_AABB(aabb);
		}

		AABB get_AABB() { return aabb; }

		void set_shadow(bool a_shadow) { shadow = a_shadow; }
		bool cast_shadow() { return shadow; }

		void set_bounds(bool a_bounds) { bounds = a_bounds; }
		bool apply_bounds() { return bounds; }

	//~ protected:
		Material	*material;
		Transform	*trans;

		AABB		aabb;

		// Proyectar sombra?
		bool		shadow;
		// Aplicar abBox?
		bool		bounds;
};

#endif // __SHAPE_HPP__
