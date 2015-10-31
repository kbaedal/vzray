#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

#include <float.h>

#include "shapes/plane.h"
#include "onb.h"

const float Plane::kplane_epsilon = 1e-8;

bool Plane::hit(const Ray &r, float min_dist, float max_dist, HitRecord &hit) const
{
	float vd, vo, t;

    vd = dot(versor(normal), versor(r.direction()));
     if(vd != 0.0f) {
        vo = -(dot(versor(normal), Vec3(r.origin())) + dist);
        t = vo / vd;

        if((t > kplane_epsilon) && (t > min_dist) && (t < max_dist)) // Hit. Fill HitRecord.
        {
            hit.dist 	    = t;
			hit.normal      = versor(normal);
			hit.material    = material;

            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool Plane::shadow_hit(const Ray &r, float min_dist, float max_dist) const
{
	float vd, vo, t;

    vd = dot(versor(normal), versor(r.direction()));
     if(vd != 0.0f) {
        vo = -(dot(versor(normal), Vec3(r.origin())) + dist);
        t = vo / vd;

        if((t > kplane_epsilon) && (t > min_dist) && (t < max_dist)) // Hit.
        {
			return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool Plane::get_random_point(const Point &view_pos, CRandomMersenne *rng, Point &light_pos) const
{
	light_pos = Point(0.0f);
	return true;
}
