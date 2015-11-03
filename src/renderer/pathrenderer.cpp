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

const float PathRenderer::kepsilon = 1e-2; // Autohit.

RGB PathRenderer::get_color(Ray r, Scene *scene, float min_dist, float max_dist, int depth)
{
	HitRecord	hit_r;
	RGB			color, temp_color;
	float 		brdf;

	Point		intersection;
	Vec3		out_dir;
	Ray			out_ray;

	if(scene->nearest_intersection(r, min_dist, max_dist, hit_r)) {
		// Añadimos emittance
		color = color + hit_r.material->emittance();

		if(depth < max_depth) {
			// Generar un nuevo rayo, y lanzarlo a la escena.

			// Punto de intersección.
			intersection = r.get_point(hit_r.dist);

			// Dirección del nuevo rayo.
			out_dir	= hit_r.material->out_direction(r.direction(), hit_r.normal, brdf, temp_color, &rng);
			out_dir.normalize();

			// Nuevo rayo:
			out_ray	= Ray(intersection + (kepsilon * Point(out_dir)), out_dir);

			color = color + get_color(out_ray, scene, min_dist, 1e5, depth + 1) * temp_color * brdf;
		}

		return color;
	}
	else {
		return RGB(0.0f, 0.0f, 0.0f);
	}
}
