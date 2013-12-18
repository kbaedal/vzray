#ifndef __BOX_HPP__
#define __BOX_HPP__ 1

#include <ctime>

#include "vec3.h"
#include "point.h"
#include "ray.h"
#include "rgb.h"
#include "shapes/shape.h"
#include "randomc/randomc.h"

class Box : public Shape
{
	public:
		Box(const Point &a_p3Min, const Point &a_p3Max, Material *a_pMat);
		Box()
		{
			m_p3Max.set( 1.f,  1.f,  1.f);
			m_p3Min.set(-1.f, -1.f, -1.f);
			
			m_pMat = NULL;
			
			m_pTrans = new Transform;
			
			bShadow = true;
			bBounds = true;
		};
		~Box() { if(m_pTrans != NULL) delete m_pTrans; };
		
		bool hit(const Ray &a_rRay, float a_dMin, float a_dMax, HitRecord &a_hrHitRcd) const;
		bool shadowHit(const Ray &a_rRay, float a_dMin, float a_dMax) const;
		bool getRandomPoint(const Point &p3ViewPoint, CRandomMersenne *rngGen, Point &p3LPoint) const;
	
	//private:
		Point	m_p3Min, m_p3Max;
};

#endif // __BOX_HPP__
