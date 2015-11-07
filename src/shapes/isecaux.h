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
	const Ray   &r, 		// Ray to test
	const Point &minimo,	// Box min and max
	const Point &maximo,
	double      min_dist,   // Min and Max distance
	double      max_dist,
	double      &dist);     // If hit, stores distance.

// Devuelve la normal correspondiente al punto de interseccion v3Point
// con un cubo de dimensiones min<-1, -1, -1> max<1, 1, 1>
Vec3 get_box_normal(Point p);

// Resuelve una equacion cuadrática del tipo Ax² + Bx + C = 0
// En caso de haber solución, devuelve las soluciones en dT0 y dT1
// Devuelve false en caso de no haber solucion.
bool solve_quadratic(double a, double b, double c, double &t0, double &t1);

// Test ray-sphere intersection
bool test_ray_sphere(
	const Ray   &r,
	double      min_dist,
	double      max_dist,
	double 		&dist);

// Tests triangle-ray intersection
bool test_ray_triangle(
	const Ray   &r,       // Ray to test
	const Point &p0, 	// Triangle vertexs
	const Point &p1,
	const Point &p2,
	double      min_dist, 	// Min and Max distance
	double      max_dist,
	double      &dist);

// Test ray-cylinder intersection
bool test_ray_cylinder(
	const Ray   &r,
	double      min_dist,
	double      max_dist,
	double 		&dist);

// Test ray-plane intersection
bool test_ray_plane(
    const Ray   &r,
    const Point &base,
    const Vec3  &normal,
    double      min_dist,
    double      max_dist,
    double      &dist);

// Test ray-disc intersection
bool test_ray_disc(
    const Ray   &r,
    const Point &center,
    const Vec3  &normal,
    double      radius,
    double      min_dist,
    double      max_dist,
    double      &dist);

}; // namespace isecaux

#endif // __ISECAUX_H__

