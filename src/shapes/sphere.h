#ifndef __SPHERE_HPP__
#define __SPHERE_HPP__ 1

#include "shape.h"

#include "vec3.h"
#include "ray.h"
#include "rgb.h"

#include "randomc/randomc.h"

class Sphere : public Shape
{
	public:
		Sphere(Point a_p3Center, float a_dRadius, Material *a_pMat);
		Sphere()
		{
			m_p3Center 	= Point(0.f);
			m_dRadius 	= 1.f;
			m_pMat		= NULL;
			m_pTrans 	= new Transform;
			bShadow 	= true;
			bBounds		= true;
		}
		~Sphere() { if(m_pTrans != NULL) delete m_pTrans; }
		
		bool hit(const Ray &a_rRay, float a_dMin, float a_dMax, HitRecord &a_hrHitRcd) const;
		bool shadowHit(const Ray &a_rRay, float a_dMin, float a_dMax) const;
		bool getRandomPoint(const Point &p3ViewPoint, CRandomMersenne *rngGen, Point &p3LPoint) const;
	
	//private:
		Point			m_p3Center;
		float 			m_dRadius;
};	

#endif // __SPHERE_HPP__
