#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "whittedrenderer.h"

#include "material/dielectricmaterial.h"

#include "randomc/randomc.h"
#include "scene/scene.h"

const double WhittedRenderer::kepsilon    = 1e-1;

RGB WhittedRenderer::get_color(Ray r, Scene *scene, double min_dist, double max_dist, int depth)
{
	HitRecord   hit_r;
	RGB			color;

    //
    // TODO: Tratar cara a cara con materiales especulares, transmisivos, etc.
    //

    // Comprobar que tenemos una interseccion.
    if(scene->nearest_intersection(r, min_dist, max_dist, hit_r)) {
        // Añadimos el color emitido
        color += hit_r.material->emittance();

        // Y el ambiente.
        color += hit_r.material->ambient();

        if(depth < max_depth) {
            if(hit_r.material->is_light()) {
                // Devolvemos el color de la luz.
                return color += hit_r.material->radiance();
            }
            if(hit_r.material->is_specular()) {
                // Reflexión especular perfecta.

                // Calculamos el punto de interseccion.
                Point intersection { r.get_point(hit_r.dist) };

                // Dirección del nuevo rayo.
                Vec3 refl_dir { reflect(r.direction(), hit_r.normal) };
                refl_dir.normalize();

                // Nuevo rayo:
                Ray refl_ray(intersection + (kepsilon * Point(refl_dir)), refl_dir);

                return color += get_color(refl_ray, scene, min_dist, max_dist, ++depth);
            }
            if(hit_r.material->is_transmissive()) {
                // Material transmisivo.
                // Pasos:
                //  - ¿El rayo entra en el objeto o sale de el? Ojo con los indices de refraccion.
                //  - ¿Hay relfexion interna total? Si la hay, no calcular transimision, solo reflexion.
                //  - Calcular reflectancia y aplicar a lo reflejado y a lo transmitido.

                // Calculamos el punto de interseccion.
                Point intersection { r.get_point(hit_r.dist) };

                Vec3    rdir { r.direction() },
                        norm { hit_r.normal };

                double cos_t = dot(rdir, norm);
                double n1, n2;

                if(cos_t < 0.0f) { // Incoming ray
                    n1      = 1.0f;
                    n2	    = 1.5f;
                    cos_t   = -cos_t;
                }
                else { // Outgoing ray
                    n1      = 1.5f;
                    n2      = 1.0f;
                    norm    = -norm;

                    if(tir(rdir, norm, 1.5)) {
                    // Dirección del nuevo rayo.
                        Vec3 refl_dir { reflect(r.direction(), hit_r.normal) };
                        refl_dir.normalize();

                        // Nuevo rayo:
                        Ray refl_ray(intersection + (kepsilon * Point(refl_dir)), refl_dir);

                        return color += get_color(refl_ray, scene, min_dist, max_dist, ++depth);
                    }
                }

                // No hay TIR, luego calculamos fresnel y lanzamos rayo reflejado y refractado.
                double fresnel = hit_r.material->reflectance(rdir, norm, 1.5f);

                Vec3    refl_dir { reflect(rdir, norm)},
                        refr_dir { refract(rdir, norm, n1, n2)};

                refl_dir.normalize();
                refr_dir.normalize();

                Ray     refl_ray(intersection + (kepsilon * Point(refl_dir)), refl_dir),
                        refr_ray(intersection + (kepsilon * Point(refr_dir)), refr_dir);

                return color += get_color(refl_ray, scene, min_dist, max_dist, ++depth) * fresnel +
                    get_color(refr_ray, scene, min_dist, max_dist, ++depth) * (1.0f - fresnel);
            }
            else { // Superficie difusa.
                // Calculamos el punto de interseccion.
                Point intersection { r.get_point(hit_r.dist) };

                return color += this->direct_light(intersection, scene, hit_r);
            }
        }
    }
    else {
        // Si no hay interseccion, devolvemos el color de fondo.
        return scene->get_bg_color();
    }
}

Contrib WhittedRenderer::get_color_v2(Ray r, Scene *scene, double min_dist, double max_dist, int depth)
{
    return Contrib();
}

RGB WhittedRenderer::direct_light(Point p, Scene *scene, HitRecord &hit_r)
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

			/// NO SE YO...
			if(diffuse < 0.0) // Si negativo, normal apunta en la direccion contraria a la luz.
                diffuse = -diffuse;
            /// PUES ESO.

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
					diffuse_color += hit_r.material->radiance() * light->get_material()->radiance() * diffuse;
				}
			}
		}
	}

	return diffuse_color;
}
