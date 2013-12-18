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

Box::Box(const Point &a_p3Min, const Point &a_p3Max, Material *a_pMat)
{
	Point p3TMax, p3TMin, p3Centroid;

	// En su propio espacio, nuestro cubo tendrá siempre estas dimensiones,
	// lo que nos facilitará el calculo de intersecciones y normales.
	m_p3Max.set( 1.f,  1.f,  1.f);
	m_p3Min.set(-1.f, -1.f, -1.f);

	// Colocamos correctamente los datos, en caso de que no lo estén ya.
	p3TMax.set(
		std::max(a_p3Min.x(), a_p3Max.x()),
		std::max(a_p3Min.y(), a_p3Max.y()),
		std::max(a_p3Min.z(), a_p3Max.z())
	);

	p3TMin.set(
		std::min(a_p3Min.x(), a_p3Max.x()),
		std::min(a_p3Min.y(), a_p3Max.y()),
		std::min(a_p3Min.z(), a_p3Max.z())
	);

	m_pTrans = new Transform;

	// Escalamos nuestro cubo a la mitad, para que sus dimensiones sean
	// de 1 unidad en todas las coordenadas, y así poder aplicar correctamente
	// el resto de transformaciones.
	m_pTrans->scale(0.5, 0.5, 0.5);

	// Averiguamos las escalas en x, y, z
	m_pTrans->scale(
		fabs(p3TMax.x() - p3TMin.x()),	// Escala en x
		fabs(p3TMax.y() - p3TMin.y()),	// Escala en y
		fabs(p3TMax.z() - p3TMin.z())	// Escala en z
	);

	// Ahora averiguamos el centroide del cubo y trasladamos nuestro
	// cubo base con la información obtenida.
	p3Centroid = p3TMin + .5 * (p3TMax - p3TMin);

	m_pTrans->translate(p3Centroid);

	// Iniciamos nuestra bbox con los valores iniciales del cubo.
	m_abBox.set(Point(-1.f), Point(1.f));

	// Y la adaptamos a las transformaciones iniciales.
	m_abBox = m_pTrans->updateAABB(m_abBox);

	m_pMat = a_pMat;

	bShadow = true;
	bBounds = true;
}

bool Box::hit(const Ray &a_rRay, float a_dMin, float a_dMax, HitRecord &a_hrHitRcd) const
{
	Ray rObjSpace = m_pTrans->sceneToObject(a_rRay);

	float dTVal;

	if(testRayBox(rObjSpace, m_p3Min, m_p3Max, a_dMin, a_dMax, dTVal))
	{
		a_hrHitRcd.dDist 	= dTVal;
		a_hrHitRcd.v3Normal = versor(m_pTrans->objectNormalToScene(getBoxNormal(rObjSpace.getPoint(dTVal))));
		a_hrHitRcd.pMat		= m_pMat;

		return true;
	}

	return false;
}

bool Box::shadowHit(const Ray &a_rRay, float a_dMin, float a_dMax) const
{
	if(bShadow) {
		Ray rObjSpace = m_pTrans->sceneToObject(a_rRay);

		float dTVal;

		return testRayBox(rObjSpace, m_p3Min, m_p3Max, a_dMin, a_dMax, dTVal);
	}
	else
		return false;
}

bool Box::getRandomPoint(const Point &p3ViewPoint, CRandomMersenne *rngGen, Point &p3LPoint) const
{
    return true;
}
