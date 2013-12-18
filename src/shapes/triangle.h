#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__ 1

#include "vec3.h"
#include "ray.h"
#include "rgb.h"
#include "shapes/shape.h"

class Triangle : public Shape
{
	public:
		Triangle(const Point &a_p0, const Point &a_p1, const Point &a_p2, Material *a_pMat)
		{
			p0 			= a_p0; 
			p1 			= a_p1; 
			p2 			= a_p2; 
			m_pMat		= a_pMat;
			m_pTrans = new Transform;
		}
		Triangle()
		{
			p0.set(0.f, 0.f, 0.f);
			p1.set(1.f, 0.f, 0.f);
			p2.set(0.f, 0.f, 1.f);		
			m_pMat		= NULL;
			m_pTrans = new Transform;
		}
		
		bool hit(const Ray &a_rRay, float a_dMin, float a_dMax, HitRecord &a_hrHitRcd) const;
		bool shadowHit(const Ray &a_rRay, float a_dMin, float a_dMax) const;
		bool getRandomPoint(const Point &p3ViewPoint, CRandomMersenne *rngGen, Point &p3LPoint) const
		{
			p3LPoint = Point(0.f);
			return true;
		}
		
		Point 	p0, p1, p2;
};

#endif // __TRIANGLE_H__
