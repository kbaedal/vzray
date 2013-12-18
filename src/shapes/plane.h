#ifndef __PLANE_H__
#define __PLANE_H__ 1

#include "shape.h"

#include "vec3.h"
#include "point.h"
#include "ray.h"
#include "rgb.h"

class Plane : public Shape
{
	public:
		Plane(Vec3 a_v3Normal, float a_dDist, Material *a_pMat)
		{
			m_v3Normal 	= a_v3Normal;
			m_dDist	 	= a_dDist;
			m_pMat		= a_pMat;
			
			m_pTrans 	= new Transform;
			
			bShadow		= true;
			bBounds		= false;
		}
		Plane()
		{
			m_v3Normal 	= Vec3(0.f, 1.f, 0.f);
			m_dDist 		= 0.f;			
			m_pMat		= NULL;
			
			m_pTrans	= new Transform;
			
			bShadow		= true;
			bBounds		= false;
		}
		~Plane() { if(m_pTrans != NULL) delete m_pTrans; }
		
		bool hit(const Ray &a_rRay, float a_dMin, float a_dMax, HitRecord &a_hrHitRcd) const;
		bool shadowHit(const Ray &a_rRay, float a_dMin, float a_dMax) const;
		bool getRandomPoint(const Point &p3ViewPoint, CRandomMersenne *rngGen, Point &p3LPoint) const;
	
	//private:
		Vec3 	m_v3Normal;
		float 	m_dDist;
};	

#endif // __PLANE_H__
