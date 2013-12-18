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

// Tests ray-box intersection
bool testRayBox(
	const Ray &r, 		// Ray to test
	const Point &min,	// Box max and min
	const Point &max,  
	float dMin, 		// Max and Min distance
	float dMax, 
	float &dTval);
	
// Devuelve la normal correspondiente al punto de interseccion v3Point
// con un cubo de dimensiones min<-1, -1, -1> max<1, 1, 1>
Vec3 getBoxNormal(Point p3Point);

// Resuelve una equacion cuadrática del tipo Ax² + Bx + C = 0
// En caso de haber solución, devuelve las soluciones en dT0 y dT1
// Devuelve false en caso de no haber solucion.
bool solveQuadratic(float dA, float dB, float dC, float &dT0, float &dT1);

// Test ray-sphere intersection
bool testRaySphere(
	const Ray 	&r,
	const float min,
	const float max,
	float 		&dTVal);
	
// Tests triangle-ray intersection
bool testRayTriangle(
	const Ray &r, 			// Ray to test
	const Point &p0, 			// Triangle vertexs
	const Point &p1, 
	const Point &p2, 
	float dMin, 				// Max and Min distance
	float dMax, 
	float &dTval);
	
// Test ray-cylinder intersection
bool testRayCylinder(
	const Ray 	&r,
	const float min,
	const float max,
	float 		&dTVal);

#endif // __ISECAUX_H__

