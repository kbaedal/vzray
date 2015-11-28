#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "renderer/pathrenderer.h"
#include "randomc/randomc.h"
#include "material/dielectricmaterial.h"
#include "scene/scene.h"

const double PathRenderer::kepsilon = 1e-2; // Autohit.

RGB PathRenderer::get_color(Ray r, Scene *scene, double min_dist, double max_dist, int depth)
{
	HitRecord	hit_r;
	RGB			color,
                temp_color;
	double 		brdf;

	Point		intersection;
	Vec3		out_dir;
	Ray			out_ray;

    if(depth < max_depth) {
        if(scene->nearest_intersection(r, min_dist, max_dist, hit_r)) {
            // Añadimos emittance
            color = color + hit_r.material->emittance();

            if(!hit_r.material->is_light()) {
                // Generar un nuevo rayo, y lanzarlo a la escena.

                // Punto de intersección.
                intersection = r.get_point(hit_r.dist);

                // Dirección del nuevo rayo.
                out_dir	= hit_r.material->out_direction(r.direction(), hit_r.normal, brdf, temp_color, &rng);
                out_dir.normalize();

                // Nuevo rayo:
                out_ray	= Ray(intersection + (kepsilon * Point(out_dir)), out_dir);

                color += get_color(out_ray, scene, min_dist, 1e5, depth + 1) * temp_color * brdf;
            }

            return color;
        }
    }

    return scene->get_bg_color();
}

Contrib PathRenderer::get_color_v2(Ray r, Scene *scene, double min_dist, double max_dist, int depth)
{
    Contrib c;

    c[0] = RGB(0.0f);

    return c;
}
