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

Sphere::Sphere(Point a_p3Center, float a_dRadius, Material *a_pMat)
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
		m_pTrans = new Transform;
		
		m_pTrans->scale(a_dRadius, a_dRadius, a_dRadius);
		m_pTrans->translate(a_p3Center);
		
		// Iniciamos nuestra bbox
		m_abBox.set(Point(-1.f), Point(1.f));	
		
		// Y le aplicamos la transformación.		
		m_abBox = m_pTrans->updateAABB(m_abBox);
		
		m_p3Center 	= Point(0.f);
		m_dRadius 	= 1.f;
		m_pMat 		= a_pMat;
		bShadow		= true;
		bBounds 	= true;
}

bool Sphere::hit(const Ray &a_rRay, float a_dMin, float a_dMax, HitRecord &a_hrHitRcd) const
{	
	// Transformar el rayo
	Ray rObjSpace = m_pTrans->sceneToObject(a_rRay);
		
	float 	dTVal;
	Vec3	v3Normal;
	
	
	if(testRaySphere(rObjSpace, a_dMin, a_dMax, dTVal)) {
		v3Normal.set(rObjSpace.getPoint(dTVal));
		
		a_hrHitRcd.dDist 	= dTVal;
		a_hrHitRcd.v3Normal = versor(m_pTrans->objectNormalToScene(v3Normal));
		a_hrHitRcd.pMat		= m_pMat;
		
		return true;
	}
	
	return false;
}

bool Sphere::shadowHit(const Ray &a_rRay, float a_dMin, float a_dMax) const
{
	if(bShadow) {
		// Transformar el rayo
		Ray rObjSpace = m_pTrans->sceneToObject(a_rRay);
	
		float dTVal;
	
		return testRaySphere(rObjSpace, a_dMin, a_dMax, dTVal);
	}
	
	return false;
}

bool Sphere::getRandomPoint(const Point &p3ViewPoint, CRandomMersenne *rngGen, Point &p3LPoint) const
{
	// Convertir el punto al espacio de la esfera
	Point 	p3VPOS;
	Vec3	v3Dist, v3W, v3DirToLight;
	ONB		UVW;
	
	
	p3VPOS = m_pTrans->sceneToObject(p3ViewPoint);
	v3Dist.set(p3VPOS - m_p3Center);
	
	// En espacio de la esfera, el centro siempre es el origen.
	float d = v3Dist.length(); /** float d = Vec3(v3ViewPoint - v3Center).length(); **/
	
	if(d < m_dRadius) // Punto en el interior de la esfera.
		return false;
	
	int nCont = 0;
	do {	
		float dSeedX = rngGen->Random();
		float dSeedY = rngGen->Random();
					
		// En espacio de la esfera, el radio es siempre 1.0
		float dSinAlphaMax = m_dRadius / d; 
		float dCosAlphaMax = sqrt(1.f - dSinAlphaMax * dSinAlphaMax);
		float q = 1.f / (2.f * M_PI * (1.f - dCosAlphaMax));
		
		float dCosAlpha = 1.f + dSeedX * (dCosAlphaMax - 1.f);
		float dSinAlpha = sqrt(1.f - dCosAlpha * dCosAlpha);
		
		float dPhi = 2.f * M_PI * dSeedY;
		float dCosPhi = cos(dPhi);
		float dSinPhi = sin(dPhi);
		
		Vec3 k_i(dCosPhi * dSinAlpha, dSinPhi * dSinAlpha, dCosAlpha);
		
		v3W.set(m_p3Center - p3VPOS);
		UVW.initFromW(v3W);
		v3DirToLight = k_i.x() * UVW.u() + k_i.y() * UVW.v() + k_i.z() * UVW.w();
				
		// Tenemos el origen, v3VPOS, y la direccion del rayo, v3DirToLight.
		// Estos datos están en el espacio de la esfera, los transformamos
		// al espacio del mundo porque al pasar un rayo a la funcion hit,
		// ésta volvera a transformarlo al espacio de la esfera.
		Ray rToLight = m_pTrans->objectToScene(Ray(p3VPOS, v3DirToLight));
		
		HitRecord htRec;
		if(this->hit(rToLight, 0.01, FLT_MAX, htRec)) {
			p3LPoint = rToLight.getPoint(htRec.dDist);
	
			return true;
		}
		
		nCont++;		
	} while(nCont < 30);
	
	return false;
}



