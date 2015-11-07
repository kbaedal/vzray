#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

#include <float.h>

#include "disc.h"
#include "isecaux.h"

#include "onb.h"

#include "statistics.h"

const double Disc::kdisc_epsilon = 1e-8;

bool Disc::hit(const Ray &r, double min_dist, double max_dist, HitRecord &hit_r) const
{
    double t;

    if(isecaux::test_ray_disc(r, center, normal, radius, min_dist, max_dist, t)) {
        hit_r.dist 	    = t;
        hit_r.normal    = normal;
        hit_r.material  = material;

        return true;
    }

    return false;
}

bool Disc::shadow_hit(const Ray &r, double min_dist, double max_dist) const
{
    double t;

    return isecaux::test_ray_disc(r, center, normal, radius, min_dist, max_dist, t);
}

bool Disc::get_random_point(const Point &view_pos, CRandomMersenne *rng, Point &light_pos) const
{
    // TODO
    // Generar un punto en el interior del disco.
	light_pos = center;

	return true;
}
