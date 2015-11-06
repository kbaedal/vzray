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

	if(depth < max_depth) {
		if(scene->nearest_intersection(r, min_dist, max_dist, hit_r)) {
			if(hit_r.material->is_light()) { // El objeto es una luz. Devolvemos su color.
				//cout << "Trinchada una luz." << endl << flush;
				return hit_r.material->emittance();
			}
			else {
				// Obtenemos el punto de intersección
				intersection = r.get_point(hit_r.dist);

				if(hit_r.material->is_specular()) {
					// Obtenemos la nueva direccion a seguir.
					out_dir = hit_r.material->out_direction(r.direction(), hit_r.normal, brdf, temp_color, &rng);
					out_dir.normalize();

					// Calculamos nuesto nuevo rayo.
					out_ray = Ray(intersection + kepsilon * Point(out_dir), out_dir);

					return get_color(out_ray, scene, min_dist, 1e5, depth + 1) * temp_color * brdf;
				}
				else if(hit_r.material->is_transmissive()) {
                    DielectricMaterial *dielectric = (DielectricMaterial *)hit_r.material;

					if(dielectric->is_TIR(r.direction(), hit_r.normal)) { // Reflexión interna total.
						out_dir = dielectric->reflect_dir(r.direction(), hit_r.normal, brdf, temp_color);
						out_dir.normalize();

						out_ray = Ray(intersection + kepsilon * Point(out_dir), out_dir);

						return get_color(out_ray, scene, min_dist, 1e5, depth + 1) * temp_color;
					}
					else { // Aplicamos leyes de snell para reflexión y refracción.
                        Vec3    out_dir_transmission,
                                out_dir_specular;
                        Ray     out_ray_transmission,
                                out_ray_specular;
                        double  brdf_transmission,
                                brdf_specular;
                        RGB     color_transmission,
                                color_specular;

						out_dir_transmission = dielectric->refract_dir(r.direction(), hit_r.normal, brdf_transmission, color_transmission);
						out_dir_transmission.normalize();

						out_ray_transmission = Ray(intersection + (kepsilon * Point(out_dir_transmission)), out_dir_transmission);

						out_dir_specular = dielectric->reflect_dir(r.direction(), hit_r.normal, brdf_specular, color_specular);
						out_dir_specular.normalize();

						out_ray_specular = Ray(intersection + (kepsilon * Point(out_dir_specular)), out_dir_specular);

						return get_color(out_ray_transmission, scene, min_dist, 1e5, depth + 1) * color_transmission * brdf_transmission +
							get_color(out_ray_specular, scene, min_dist, 1e5, depth + 1) * color_specular * brdf_specular;
					}

				}
				else { // Superficie difusa.
					// Calculamos el suavizado de sombras solo en los rayos primarios.
					if(depth < 2) {
						for(int i = 0; i < shadow_samps; i++)
							color = color + direct_light(r.get_point(hit_r.dist), scene, hit_r) * 1.0f/shadow_samps;
					}
					else {
						color = color + direct_light(r.get_point(hit_r.dist), scene, hit_r);
					}

					return color;
				}
			}
		}
		else {
			// No hay intersección. Devolvemos el color de fondo.
			return scene->get_bg_color();
		}
	}
	else // If max_depth, return black.
		return RGB(0.0, 0.0, 0.0);
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
				j++;
			}
			if(!shadow_hit) {
				if(hit.material != NULL) {
					Vec2 v2(0.0f, 0.0f);
					Vec3 v3(0.0f, 0.0f, 0.0f);
					diffuse_color = hit.material->radiance() * scene->get_light(i)->get_material()->radiance() * diffuse;
				}
			}
		}
	}

	return diffuse_color;
}
