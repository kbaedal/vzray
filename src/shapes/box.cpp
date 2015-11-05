#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#include <float.h>

#include "shapes/isecaux.h"
#include "shapes/box.h"

#include "vec3.h"

Box::Box(const Point &a_minimo, const Point &a_maximo, Material *a_material)
{
	Point   temp_max,
            temp_min,
            centroid;

	// En su propio espacio, nuestro cubo tendrá siempre estas dimensiones,
	// lo que nos facilitará el calculo de intersecciones y normales.
	maximo.set( 1.f,  1.f,  1.f);
	minimo.set(-1.f, -1.f, -1.f);

	// Colocamos correctamente los datos, en caso de que no lo estén ya.
	temp_max.set(
		std::max(a_minimo.x(), a_maximo.x()),
		std::max(a_minimo.y(), a_maximo.y()),
		std::max(a_minimo.z(), a_maximo.z())
	);

	temp_min.set(
		std::min(a_minimo.x(), a_maximo.x()),
		std::min(a_minimo.y(), a_maximo.y()),
		std::min(a_minimo.z(), a_maximo.z())
	);

	trans = new Transform;

	// Escalamos nuestro cubo a la mitad, para que sus dimensiones sean
	// de 1 unidad en todas las coordenadas, y así poder aplicar correctamente
	// el resto de transformaciones.
	trans->scale(0.5, 0.5, 0.5);

	// Averiguamos las escalas en x, y, z
	trans->scale(
		fabs(temp_max.x() - temp_min.x()),	// Escala en x
		fabs(temp_max.y() - temp_min.y()),	// Escala en y
		fabs(temp_max.z() - temp_min.z())	// Escala en z
	);

	// Ahora averiguamos el centroide del cubo y trasladamos nuestro
	// cubo base con la información obtenida.
	centroid = temp_min + .5 * (temp_max - temp_min);

	trans->translate(centroid);

	// Iniciamos nuestra bbox con los valores iniciales del cubo.
	aabb.set(Point(-10.f), Point(1.0f));

	// Y la adaptamos a las transformaciones iniciales.
	aabb = trans->update_AABB(aabb);

	material = a_material;

	shadow = true;
	bounds = true;
}

bool Box::hit(const Ray &r, double min_dist, double max_dist, HitRecord &hit) const
{
	Ray r_obj_space = trans->scene_to_object(r);

	double dist;

	if(isecaux::test_ray_box(r_obj_space, minimo, maximo, min_dist, max_dist, dist))
	{
		hit.dist 	    = dist;
		hit.normal      = versor(trans->normal_to_scene(isecaux::get_box_normal(r_obj_space.get_point(dist))));
		hit.material    = material;

		return true;
	}

	return false;
}

bool Box::shadow_hit(const Ray &r, double min_dist, double max_dist) const
{
	if(shadow) {
		Ray r_obj_space = trans->scene_to_object(r);

		double dist;

		return isecaux::test_ray_box(r_obj_space, minimo, maximo, min_dist, max_dist, dist);
	}
	else
		return false;
}

bool Box::get_random_point(const Point &view_pos, CRandomMersenne *rng, Point &light_pos) const
{
    return true;
}
