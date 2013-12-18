#ifndef __PARALLELOGRAM_H__
#define __PARALLELOGRAM_H__ 1

#include "shape.h"

#include "vec3.h"
#include "point.h"
#include "ray.h"
#include "rgb.h"


class Parallelogram : public Shape
{
	public:
		Parallelogram(Point a_p3Base, Vec3 a_v3u, Vec3 a_v3v, Material *a_pMat)
		{
			m_p3Base	= a_p3Base;
			m_v3u		= a_v3u;
			m_v3v		= a_v3v;			
			m_pMat		= a_pMat;
			
			m_v3Normal 	= versor(cross(m_v3u, m_v3v));
		
			m_v3uNorm = versor(m_v3u);
			m_v3vNorm = versor(m_v3v);
			
			m_pTrans = new Transform;
		}
		~Parallelogram() { if(m_pTrans != NULL) delete m_pTrans; }
						
		bool hit(const Ray &a_rRay, float a_dMin, float a_dMax, HitRecord &a_hrHitRcd) const;
		bool shadowHit(const Ray &a_rRay, float a_dMin, float a_dMax) const;
		bool getRandomPoint(const Point &p3ViewPoint, CRandomMersenne *rngGen, Point &p3LPoint) const;
	
	//private:
		Point 	m_p3Base;
		
		Vec3 	m_v3u,
				m_v3v,
				m_v3Normal,
				m_v3uNorm,
				m_v3vNorm;
};	

#endif // __PARALLELOGRAM_H__
