#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "renderer/pathrenderer.h"
#include "randomc/randomc.h"
#include "material/dielectricmaterial.h"
#include "scene/scene.h"

const float dEpsilon = 1e-2; // Avoid auto-hit.

RGB PathRenderer::getColor(Ray a_rRay, Scene *a_Scene, float a_dMin, float a_dMax, int a_nDepth)
{
	HitRecord	htHit;
	RGB			rgbColor, rgbTemp;	
	float 		dBRDF;
	
	Point		p3IntPoint, p3OutDir;
	Vec3		v3OutDir;
	Ray			rOutRay;
				
	if(a_Scene->nearestIntersection(a_rRay, a_dMin, a_dMax, htHit)) {
		// Añadimos emittance
		rgbColor = rgbColor + htHit.pMat->emittance();
						
		if(a_nDepth < m_nMaxDepth) {
			// Generar un nuevo rayo, y lanzarlo a la escena.
			
			// Punto de intersección.
			p3IntPoint 	= a_rRay.getPoint(htHit.dDist);
			
			// Dirección del nuevo rayo.
			v3OutDir 	= htHit.pMat->outDirection(a_rRay.direction(), htHit.v3Normal, dBRDF, rgbTemp, &m_MyRNG);
			v3OutDir.normalize();
			
			// Evitamos el autohit.
			p3OutDir.set(v3OutDir);
			
			// Nuevo rayo:
			rOutRay	= Ray(p3IntPoint + (dEpsilon * p3OutDir), v3OutDir);
			
			rgbColor = rgbColor + this->getColor(rOutRay, a_Scene, a_dMin, 1e5, a_nDepth + 1) * rgbTemp * dBRDF;
		}
		
		return rgbColor;
	}
	else {
		return RGB(0.0, 0.0, 0.0);
	}
}
