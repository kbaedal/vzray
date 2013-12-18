#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

#include <float.h>

#include "shapes/plane.h"
#include "onb.h"

const float PLANE_EPSILON = 1e-8;

using namespace std;

bool Plane::hit(const Ray &a_rRay, float a_dMin, float a_dMax, HitRecord &a_hrHitRcd) const
{	  
	float 	Vd, Vo, t;
	Vec3	v3Origin;
	
	v3Origin.set(a_rRay.origin());
			
    Vd = dot(versor(m_v3Normal), versor(a_rRay.direction()));
     if(Vd != 0.f) {
        Vo = -(dot(versor(m_v3Normal), v3Origin) + m_dDist);
        t = Vo / Vd;

        if((t > PLANE_EPSILON) && (t > a_dMin) && (t < a_dMax)) // Hit. Fill HitRecord.
        {
            a_hrHitRcd.dDist 	= t;
			a_hrHitRcd.v3Normal = versor(m_v3Normal);			
			a_hrHitRcd.pMat		= m_pMat;
			
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }	
}

bool Plane::shadowHit(const Ray &a_rRay, float a_dMin, float a_dMax) const
{
	float 	Vd, Vo, t;
	Vec3	v3Origin;
	
	v3Origin.set(a_rRay.origin());
	
    Vd = dot(versor(m_v3Normal), versor(a_rRay.direction()));
    if(Vd != 0.f) {
        Vo = -(dot(versor(m_v3Normal), v3Origin) + m_dDist);
        t = Vo / Vd;

        if((t > PLANE_EPSILON) && (t > a_dMin) && (t < a_dMax)) // Hit.
        {
			return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }    
}

bool Plane::getRandomPoint(const Point &p3ViewPoint, CRandomMersenne *rngGen, Point &p3LPoint) const
{
	p3LPoint = Point(0.f);
	return true;
}
