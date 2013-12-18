#ifndef __CYLINDER_HPP__
#define __CYLYNDER_HPP__ 1

#include "shape.h"

#include "vec3.h"
#include "point.h"
#include "ray.h"
#include "rgb.h"
#include "randomc/randomc.h"

class Cylinder : public Shape
{
	public:
		Cylinder(Point a_p3P0, Point a_p3P1, float a_dRadius, Material *a_pMat);
		Cylinder()
		{
			m_p3P0 		= Point( 0.f, 0.f, -1.f);
			m_p3P1 		= Point( 0.f, 0.f,  1.f);
			m_dRadius  	= 1.f;
			m_pMat		= NULL;
			m_pTrans 	= new Transform;
			bShadow 	= true;
			bBounds		= true;
		}
		~Cylinder() { if(m_pTrans != NULL) delete m_pTrans; }
		
		bool hit(const Ray &a_rRay, float a_dMin, float a_dMax, HitRecord &a_hrHitRcd) const;
		bool shadowHit(const Ray &a_rRay, float a_dMin, float a_dMax) const;
		bool getRandomPoint(const Point &p3ViewPoint, CRandomMersenne *rngGen, Point &p3LPoint) const;
	
	//private:
		Point 			m_p3P0, m_p3P1;
		float 			m_dRadius;
};	

#endif // __CYLINDER_HPP__

