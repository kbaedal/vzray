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
	double		brdf = 1.0;
	Vec3		out_dir;
    Point		intersection;
    Ray			out_ray;

    //
    // TODO: Tratar cara a cara con materiales especulares, transmisivos, etc.
    //

    // Comprobar que tenemos una interseccion.
    if(scene->nearest_intersection(r, min_dist, max_dist, hit_r)) {
        // Añadimos el color emitido
        color += hit_r.material->emittance();

        // Y el ambiente.
        color += hit_r.material->ambient();

        // Calculamos el punto de interseccion.
        intersection = r.get_point(hit_r.dist);


        //
        // OJO A LO QUE TIENE QUE MULTPLICARSE O SUMARSE.
        // No me fio un pelo del brdf ni de la luz directa.
        //
        if(depth < max_depth) {
            // Veamos la direccion a la que enviamos la luz
            out_dir = hit_r.material->out_direction(r.direction(), hit_r.normal, brdf, temp_color, &rng);

            out_ray.set(intersection, out_dir);
            color += temp_color * get_color(out_ray, scene, min_dist, max_dist, depth+1) * brdf;;

            if( !(hit_r.material->is_specular() || hit_r.material->is_specular()) )
                color += direct_light(intersection, scene, hit_r);
        }

        return color;
    }
    else {
        // Si no hay interseccion, devolvemos el color de fondo.
        return scene->get_bg_color();
    }
}

Contrib DirectRenderer::get_color_v2(Ray r, Scene *scene, double min_dist, double max_dist, int depth)
{
    HitRecord   hit_r;
	RGB			color,
                temp_color;
	double		brdf = 1.0;
	Vec3		out_dir;
    Point		intersection;
    Ray			out_ray;

    Contrib c, temp_contrib;

    //
    // TODO: Tratar cara a cara con materiales especulares, transmisivos, etc.
    //

    // Comprobar que tenemos una interseccion.
    if(scene->nearest_intersection(r, min_dist, max_dist, hit_r)) {
        // Añadimos el color emitido
        if(depth == 1)
            c[1] += hit_r.material->emittance();
        else
            c[2] += hit_r.material->emittance();


        if(depth == 1)// Y el ambiente.
            c[1] += hit_r.material->ambient();
        else
            c[2] += hit_r.material->ambient();

        // Calculamos el punto de interseccion.
        intersection = r.get_point(hit_r.dist);

        if(depth < max_depth) {
            // Veamos la direccion a la que enviamos la luz
            out_dir = hit_r.material->out_direction(r.direction(), hit_r.normal, brdf, temp_color, &rng);

            out_ray.set(intersection, out_dir);
            c += temp_contrib * get_color_v2(out_ray, scene, min_dist, max_dist, depth+1);

        }

        if(depth == 1)
            c[1] += direct_light(intersection, scene, hit_r) * brdf;
        else
            c[2] += direct_light(intersection, scene, hit_r) * brdf;

        return c;
    }
    else {
        // Si no hay interseccion, devolvemos el color de fondo.
        if(depth == 1)
            c[1] = scene->get_bg_color();
        else
            c[2] = scene->get_bg_color();

        return c;
    }
}

RGB DirectRenderer::direct_light(Point p, Scene *scene, HitRecord &hit_r)
{
	RGB 	diffuse_color;
	Point 	light_point;
	Vec3	light_dir;
	Ray 	shadow_ray;
	double 	light_dist,
            diffuse;
	bool 	shadow_hit;

	for(int i = 0; i < scene->get_num_lights(); i++) {
        Shape *light = scene->get_light(i);

		if(light->get_random_point(p, &rng, light_point)) {
			// Direccion desde el punto de interseccion a la luz a testear.
			light_dir.set(light_point - p);
			// Distancia entre ambos puntos.
			light_dist = light_dir.length();
			// Vector normalizado, para el rayo de testeo.
			light_dir.normalize();

			shadow_ray.set(p + kepsilon * Point(light_dir), light_dir);

            // Se supone que tanto la normal como la direccion están normalizados.
			diffuse = dot(hit_r.normal, shadow_ray.direction());

			shadow_hit = false;
			int j = 0;
			while(!shadow_hit && j < scene->get_num_objs()) {
                Shape *test_obj = scene->get_object(j);
				if(test_obj != light) {
					if(test_obj->shadow_hit(shadow_ray, kepsilon, light_dist))
						shadow_hit = true;
				}
				++j;
			}
			if(!shadow_hit) {
				if(hit_r.material != NULL) {
					Vec2 v2(0.0f);
					Vec3 v3(0.0f);
					diffuse_color = hit_r.material->radiance() * light->get_material()->radiance() * diffuse;
				}
			}
		}
	}

	return diffuse_color;
}
