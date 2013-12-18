#ifndef __MESHTRIANGLE_HPP__
#define __MESHTRIANGLE_HPP__ 1

#include "shape.h"
#include "mesh.h"

#include "vec3.h"
#include "ray.h"
#include "rgb.h"

class MeshTriangle : public Shape 
{	
	public:	
		MeshTriangle(const MeshFace &mfInfo, Mesh *a_pMesh);
		
		bool hit(const Ray &a_rRay, float a_dMin, float a_dMax, HitRecord &a_hrHitRcd) const;
		bool shadowHit(const Ray &a_rRay, float a_dMin, float a_dMax) const;
		bool getRandomPoint(const Point &p3ViewPoint, CRandomMersenne *rngGen, Point &p3LPoint) const
		{
			p3LPoint = Point(0.f);
			return true;
		}
	
	private:
		// Lista de índices (vertices, normales, texturas)
		int		nVList[3];
		int		nNList[3];
		int		nTList[3];
		
		// Puntero al objeto mesh al que pertenece el triángulo
		Mesh	*pMesh;
};

#endif // __MESHTRIANGLE_HPP__
