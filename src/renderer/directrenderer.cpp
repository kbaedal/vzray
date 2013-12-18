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

const float dEpsilon = 1e-2; // Avoid auto-hit.


RGB DirectRenderer::getColor(Ray a_rRay, Scene *a_Scene, float a_dMin, float a_dMax, int a_nDepth)
{
	HitRecord			htHit;
	RGB					rgbColor, rgbTemp, rgbTemp_T, rgbTemp_S;
	float				dBRDF, dBRDF_T, dBRDF_S;
	DielectricMaterial	*pDMat;
	
	Vec3		v3OutDir, v3OutDir_T, v3OutDir_S;
	Point		p3IntPoint,
				p3OutDir, p3OutDir_T, p3OutDir_S;
	Ray			rOutRay, rOutRay_T, rOutRay_S;
	
	if(a_nDepth < m_nMaxDepth) {		
		if(a_Scene->nearestIntersection(a_rRay, a_dMin, a_dMax, htHit)) {
			if(htHit.pMat->isLight()) { // El objeto es una luz. Devolvemos su color.
				//cout << "Trinchada una luz." << endl << flush;
				return htHit.pMat->emittance();			
			}
			else {
				// Obtenemos el punto de intersección
				p3IntPoint = a_rRay.getPoint(htHit.dDist);
					
				if(htHit.pMat->isSpecular()) {
					// Obtenemos la nueva direccion a seguir.				
					v3OutDir = htHit.pMat->outDirection(a_rRay.direction(), htHit.v3Normal, dBRDF, rgbTemp, &m_MyRNG);
					v3OutDir.normalize();
									
					// Calculamos nuesto nuevo rayo.
					p3OutDir.set(v3OutDir);
					rOutRay = Ray(p3IntPoint + dEpsilon * p3OutDir, v3OutDir);
				  
					return this->getColor(rOutRay, a_Scene, a_dMin, 1e5, a_nDepth + 1) * rgbTemp * dBRDF;
				} 
				else if(htHit.pMat->isTransmissive()) {
					pDMat = (DielectricMaterial *)htHit.pMat;
					
					if(pDMat->isTIR(a_rRay.direction(), htHit.v3Normal)) { // Reflexión interna total.
						v3OutDir = pDMat->reflectDir(a_rRay.direction(), htHit.v3Normal, dBRDF, rgbTemp);
						v3OutDir.normalize();
						
						p3OutDir.set(v3OutDir);
						rOutRay = Ray(p3IntPoint + dEpsilon * p3OutDir, v3OutDir);
						
						return this->getColor(rOutRay, a_Scene, a_dMin, 1e5, a_nDepth + 1) * rgbTemp;
					} 
					else { // Aplicamos leyes de snell para reflexión y refracción.
						v3OutDir_T	= pDMat->refractDir(a_rRay.direction(), htHit.v3Normal, dBRDF_T, rgbTemp_T);
						v3OutDir_T.normalize();
						
						p3OutDir_T.set(v3OutDir_T);
						rOutRay_T = Ray(p3IntPoint + (dEpsilon * p3OutDir_T), v3OutDir_T);
						
						v3OutDir_S	= pDMat->reflectDir(a_rRay.direction(), htHit.v3Normal, dBRDF_S, rgbTemp_S);
						v3OutDir_S.normalize();
						
						p3OutDir_S.set(v3OutDir_S);
						rOutRay_S = Ray(p3IntPoint + (dEpsilon * p3OutDir_S), v3OutDir_S);
						
						return this->getColor(rOutRay_T, a_Scene, a_dMin, 1e5, a_nDepth + 1) * rgbTemp_T * dBRDF_T +
							this->getColor(rOutRay_S, a_Scene, a_dMin, 1e5, a_nDepth + 1) * rgbTemp_S * dBRDF_S;
					}
				
				}			
				else { // Superficie difusa.
					// Calculamos el suavizado de sombras solo en los rayos primarios.
					if(a_nDepth < 2) { 
						for(int i = 0; i < m_nShadowSamps; i++)
							rgbColor = rgbColor + directLight(a_rRay.getPoint(htHit.dDist), a_Scene, htHit) * 1.f/m_nShadowSamps;
					}
					else {
						rgbColor = rgbColor + directLight(a_rRay.getPoint(htHit.dDist), a_Scene, htHit);
					}
					
					return rgbColor;
				}
			} 
		} 
		else {
			// No hay intersección. Devolvemos el color de fondo.
			return a_Scene->getBGColor();
		}
	}
	else // If max_depth, return black.
		return RGB(0.0, 0.0, 0.0);
}

RGB DirectRenderer::directLight(Point p3Point, Scene *a_Scene, HitRecord &htHit)
{
	RGB 	rgbDiffColor;	
	Point 	p3LitePoint, p3LiteDir;
	Vec3	v3LiteDir;
	Ray 	rShadowRay;
	float 	dLiteDist, dDiffuse;
	bool 	bShadowHit;

	for(int i = 0; i < a_Scene->getNumLights(); i++) {		
		
		if(a_Scene->getLight(i)->getRandomPoint(p3Point, &m_MyRNG, p3LitePoint)) {
			// Direccion desde el punto de interseccion a la luz a testear.
			v3LiteDir.set(p3LitePoint - p3Point);
			// Distancia entre ambos puntos.
			dLiteDist = v3LiteDir.length();
			// Vector normalizado, para el rayo de testeo.
			v3LiteDir.normalize();
			// Auxiliar para evitar el autohit.
			p3LiteDir.set(v3LiteDir);
			
			rShadowRay.set(p3Point + dEpsilon * p3LiteDir, v3LiteDir);
			
			dDiffuse = dot(versor(htHit.v3Normal), versor(rShadowRay.direction()));
			
			bShadowHit = false;
			int j = 0;
			while(!bShadowHit && j < a_Scene->getNumObjs()) {
				if(a_Scene->getObject(j) != a_Scene->getLight(i)) {
					if(a_Scene->getObject(j)->shadowHit(rShadowRay, dEpsilon, dLiteDist))
						bShadowHit = true;
				}
				j++;
			}
			if(!bShadowHit) {
				if(htHit.pMat != NULL) {
					Vec2 UnV2(0., 0.);
					Vec3 UnV3(0., 0., 0.);
					rgbDiffColor = htHit.pMat->radiance() * a_Scene->getLight(i)->getMaterial()->radiance() * dDiffuse;
				}
			}
		}
	}
	
	return rgbDiffColor;
}
