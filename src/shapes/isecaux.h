#ifndef __ISECAUX_H__
#define __ISECAUX_H__ 1

//
//
// Auxiliar functions for intersect test and related
//
//

#include "ray.h"
#include "vec3.h"
#include "point.h"

namespace isecaux {

// Tests ray-box intersection
bool test_ray_box(
	const Ray &r, 		    // Ray to test
	const Point &minimo,	// Box min and max
	const Point &maximo,
	float min_dist, 		// Min and Max distance
	float max_dist,
	float &dist);

// Devuelve la normal correspondiente al punto de interseccion v3Point
// con un cubo de dimensiones min<-1, -1, -1> max<1, 1, 1>
Vec3 get_box_normal(Point p);

// Resuelve una equacion cuadrática del tipo Ax² + Bx + C = 0
// En caso de haber solución, devuelve las soluciones en dT0 y dT1
// Devuelve false en caso de no haber solucion.
bool solve_quadratic(float a, float b, float c, float &t0, float &t1);

// Test ray-sphere intersection
bool test_ray_sphere(
	const Ray 	&r,
	const float min_dist,
	const float max_dist,
	float 		&dist);

// Tests triangle-ray intersection
bool test_ray_triangle(
	const Ray &r,       // Ray to test
	const Point &p0, 	// Triangle vertexs
	const Point &p1,
	const Point &p2,
	float min_dist, 	// Min and Max distance
	float max_dist,
	float &dist);

// Test ray-cylinder intersection
bool test_ray_cylinder(
	const Ray 	&r,
	const float min_dist,
	const float max_dist,
	float 		&dist);

}; // namespace isecaux

#endif // __ISECAUX_H__

