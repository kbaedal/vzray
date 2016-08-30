#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#include <float.h>

#include "sphere.h"
#include "isecaux.h"

#include "onb.h"
#include "randomc/randomc.h"

Sphere::Sphere(Point a_center, double a_radius, Material *a_material)
{
		// El radio lo aplicaremos como una transformacion de escalado
		// y la posicion como una transformación de traslacion, ya que
		// en el espacio de la esfera ésta siempre está en el origen
		// y tiene radio 1. Dadas las caracterísiticas de la esfera,
		// la bbox que la contiene será:
		//	- min: <-1.0, -1.0, -1.0>
		//	- max: < 1.0,  1.0,  1.0>
		// Una vez creada la bbox con estos valores, que están en el
		// espacio de la esfera, le aplicaremos la transformación para
		// obtener la bbox en el espacio real.

		// Creamos la matriz de transformación
		trans = new Transform;

		trans->scale(a_radius, a_radius, a_radius);
		trans->translate(a_center);

		// Iniciamos nuestra bbox
		aabb.set(Point(-1.0f), Point(1.0f));

		// Y le aplicamos la transformación.
		aabb = trans->update_AABB(aabb);

		center 	    = Point(0.0f);
		radius 	    = 1.0f;
		material    = a_material;
		shadow		= true;
		bounds 	    = true;
}

bool Sphere::hit(const Ray &r, double min_dist, double max_dist, HitRecord &hit) const
{
	// Transformar el rayo
	Ray r_obj_space = trans->scene_to_object(r);

	double 	dist;
	Vec3	normal;

	if(isecaux::test_ray_sphere(r_obj_space, min_dist, max_dist, dist)) {
		normal = r_obj_space.get_point(dist);

		hit.dist 	    = dist;
		hit.normal      = versor(trans->normal_to_scene(normal));
		hit.material    = material;

		return true;
	}

	return false;
}

bool Sphere::shadow_hit(const Ray &r, double min_dist, double max_dist) const
{
	if(shadow) {
		// Transformar el rayo
		Ray r_obj_space = trans->scene_to_object(r);

		double dist;

		return isecaux::test_ray_sphere(r_obj_space, min_dist, max_dist, dist);
	}

	return false;
}

bool Sphere::get_random_point(const Point &view_pos, CRandomMersenne *rng, Point &light_pos) const
{
    // Convertir el punto al espacio de la esfera
	Point 	pos;
	Vec3	w, dir_to_light;
	ONB		uvw;

	pos = trans->scene_to_object(view_pos);

	// En espacio de la esfera, el centro siempre es el origen.
	double d = Vec3(pos).length();

	if(d < radius) // Punto en el interior de la esfera.
		return false;

	int cont = 0;
	do {
		double seed_x = rng->Random();
		double seed_y = rng->Random();

		// En espacio de la esfera, el radio es siempre 1.0
		double sin_alpha_max = radius / d;
		double cos_alpha_max = sqrt(1.0f - sin_alpha_max * sin_alpha_max);
		//double q = 1.f / (2.f * M_PI * (1.f - dCosAlphaMax));

		double cos_alpha = 1.0f + seed_x * (cos_alpha_max - 1.0f);
		double sin_alpha = sqrt(1.0f - cos_alpha * cos_alpha);

		double phi = 2.0f * M_PI * seed_y;
		double cos_phi = cos(phi);
		double sin_phi = sin(phi);

		Vec3 k_i(cos_phi * sin_alpha, sin_phi * sin_alpha, cos_alpha);

		w = center - pos;
		uvw.init_from_w(w);
		dir_to_light = k_i.x() * uvw.u() + k_i.y() * uvw.v() + k_i.z() * uvw.w();

		// Tenemos el origen, pos, y la direccion del rayo, dir_to_light.
		// Estos datos están en el espacio de la esfera, los transformamos
		// al espacio del mundo porque al pasar un rayo a la funcion hit,
		// ésta volvera a transformarlo al espacio de la esfera.
		Ray r_to_light = trans->object_to_scene(Ray(pos, dir_to_light));

		HitRecord hit_r;
		if(hit(r_to_light, 0.01, FLT_MAX, hit_r)) {
			light_pos = r_to_light.get_point(hit_r.dist);

			return true;
		}

		cont++;
	} while(cont < 30);

	return false;
}





















