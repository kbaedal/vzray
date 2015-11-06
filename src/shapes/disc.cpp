#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

#include <float.h>

#include "shapes/disc.h"
#include "onb.h"
#include "ray.h"

#include "statistics.h"

const double Disc::kdisc_epsilon = 1e-8;

bool Disc::hit(const Ray &r, double min_dist, double max_dist, HitRecord &hit_r) const
{
    ++Statistics::num_prim_tests;

    double vd, vo, t;

    double dist = Vec3(center - Point(0.0f)).length();

    vd = dot(versor(normal), versor(r.direction()));
     if(vd != 0.0f) {
        vo = -(dot(versor(normal), Vec3(r.origin())) + dist);
        t = vo / vd;

        if((t > kdisc_epsilon) && (t > min_dist) && (t < max_dist)) // Hit. Fill HitRecord.
        {
            hit_r.dist 	    = t;
			hit_r.normal    = versor(normal);
			hit_r.material  = material;

            ++Statistics::num_prim_isecs;
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

bool Disc::shadow_hit(const Ray &r, double min_dist, double max_dist) const
{
    double vd, vo, t;

    double dist = Vec3(center - Point(0.0f)).length();

    vd = dot(versor(normal), versor(r.direction()));
     if(vd != 0.0f) {
        vo = -(dot(versor(normal), Vec3(r.origin())) + dist);
        t = vo / vd;

        if((t > kdisc_epsilon) && (t > min_dist) && (t < max_dist)) // Hit.
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

bool Disc::get_random_point(const Point &view_pos, CRandomMersenne *rng, Point &light_pos) const
{
	light_pos = Point(0.0f);
	return true;
}
