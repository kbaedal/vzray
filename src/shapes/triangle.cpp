#include "shapes/isecaux.h"
#include "shapes/triangle.h"

bool Triangle::hit(const Ray &a_rRay, float a_dMin, float a_dMax, HitRecord &a_hrHitRcd) const
{	
	float 	dTval;
	Vec3	v3Dir10, v3Dir20, v3TNorm;
	
	if(testRayTriangle(a_rRay, p0, p1, p2, a_dMin, a_dMax, dTval))	{
		v3Dir10.set(p1-p0);
		v3Dir20.set(p2-p0);
		
		v3TNorm = cross(v3Dir10, v3Dir20);
		v3TNorm.normalize();
		
		a_hrHitRcd.dDist 	= dTval;
		a_hrHitRcd.v3Normal = v3TNorm;
		a_hrHitRcd.pMat		= m_pMat;
		
		return true;
	}
	
	return false;
}

bool Triangle::shadowHit(const Ray &a_rRay, float a_dMin, float a_dMax) const
{
	float dTval;
	
	return testRayTriangle(a_rRay, p0, p1, p2, a_dMin, a_dMax, dTval);
}
