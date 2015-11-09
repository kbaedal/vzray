#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

#include <float.h>

#include "shapes/plane.h"
#include "onb.h"

#include "isecaux.h"

#include "statistics.h"

bool Plane::hit(const Ray &r, double min_dist, double max_dist, HitRecord &hit_r) const
{
    double t;

    if(isecaux::test_ray_plane(r, base, normal, min_dist, max_dist, t)) {
        hit_r.dist 	    = t;
        hit_r.normal    = normal;
        hit_r.material  = material;

        return true;
    }

    return false;
}

bool Plane::shadow_hit(const Ray &r, double min_dist, double max_dist) const
{
	double t;

    if(isecaux::test_ray_plane(r, base, normal, min_dist, max_dist, t))
        return true;

    return false;
}

bool Plane::get_random_point(const Point &view_pos, CRandomMersenne *rng, Point &light_pos) const
{
    // TODO
    // Generar un punto aleatorio en la superficie del plano.
	light_pos = base;
	return true;
}
