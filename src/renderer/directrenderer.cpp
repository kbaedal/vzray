#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "directrenderer.h"

#include "randomc/randomc.h"
#include "material/dielectricmaterial.h"
#include "scene/scene.h"

const double DirectRenderer::kepsilon    = 1e-2;

RGB DirectRenderer::get_color(Ray r, Scene *scene, double min_dist, double max_dist, int depth)
{
	HitRecord   hit_r;
	RGB			color,
                temp_color;
	double		brdf;
	Vec3		out_dir;
    Point		intersection;
    Ray			out_ray;

    //
    // TODO: Tratar cara a cara con materiales especulares, transmisivos, etc.
    //

    // Comprobar que tenemos una interseccion.
    if(scene->nearest_intersection(r, min_dist, max_dist, hit_r)) {
        // Añadimos el color emitido
        color = color + hit_r.material->emittance();

        // Y el ambiente.
        color = color + hit_r.material->ambient();

        if(depth < max_depth) {
            intersection = r.get_point(hit_r.dist);
            // Veamos la direccion a la que enviamos la luz
            out_dir = hit_r.material->out_direction(r.direction(), hit_r.normal, brdf, temp_color, &rng);

            out_ray.set(intersection, out_dir);
            color = color + temp_color * get_color(out_ray, scene, min_dist, max_dist, depth+1);
        }
        color = color + direct_light(intersection, scene, hit_r) * brdf;

        return color;
    }
    else {
        // Si no hay interseccion, devolvemos el color de fondo.
        return scene->get_bg_color();
    }
}

RGB DirectRenderer::direct_light(Point p, Scene *scene, HitRecord &hit)
{
	RGB 	diffuse_color;
	Point 	light_point;
	Vec3	light_dir;
	Ray 	shadow_ray;
	double 	light_dist,
            diffuse;
	bool 	shadow_hit;

	for(int i = 0; i < scene->get_num_lights(); i++) {

		if(scene->get_light(i)->get_random_point(p, &rng, light_point)) {
			// Direccion desde el punto de interseccion a la luz a testear.
			light_dir.set(light_point - p);
			// Distancia entre ambos puntos.
			light_dist = light_dir.length();
			// Vector normalizado, para el rayo de testeo.
			light_dir.normalize();

			shadow_ray.set(p + kepsilon * Point(light_dir), light_dir);

			diffuse = dot(versor(hit.normal), versor(shadow_ray.direction()));

			shadow_hit = false;
			int j = 0;
			while(!shadow_hit && j < scene->get_num_objs()) {
				if(scene->get_object(j) != scene->get_light(i)) {
					if(scene->get_object(j)->shadow_hit(shadow_ray, kepsilon, light_dist))
						shadow_hit = true;
				}
				++j;
			}
			if(!shadow_hit) {
				if(hit.material != NULL) {
					Vec2 v2(0.0f);
					Vec3 v3(0.0f);
					diffuse_color = hit.material->radiance() * scene->get_light(i)->get_material()->radiance() * diffuse;
				}
			}
		}
	}

	return diffuse_color;
}
