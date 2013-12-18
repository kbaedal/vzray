#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

#include <float.h>

#include "parallelogram.h"

#include "onb.h"

float const PARALL_EPSILON = 1e-8;

bool Parallelogram::hit(const Ray &a_rRay, float a_dMin, float a_dMax, HitRecord &a_hrHitRcd) const
{
	Point 	p3HitPlane, p3Offset;
	Vec3	v3Base, v3Origin, v3Offset;
	float	dot1, dot2, t, u1, v1;
	
	dot1 = dot(a_rRay.direction(), m_v3Normal);
	
	if(dot1 < PARALL_EPSILON && dot1 > -PARALL_EPSILON) return false;
	
	v3Base.set(m_p3Base);
	v3Origin.set(a_rRay.origin());
	
	dot2 = dot(m_v3Normal, v3Base);
	t = (dot2 - dot(m_v3Normal, v3Origin)) / dot1;
	
	if(t > a_dMax || t < a_dMin) return false;
	
	p3HitPlane = Ray(a_rRay).getPoint(t);
	v3Offset.set(p3HitPlane - m_p3Base);
	
	u1 = dot(m_v3uNorm, v3Offset) / m_v3u.length();
	if(u1 < 0.f || u1 > 1.f) return false;
	
	v1 = dot(m_v3vNorm, v3Offset) / m_v3v.length();
	if(v1 < 0.f || v1 > 1.f) return false;
	
	a_hrHitRcd.dDist 	= t;
	a_hrHitRcd.v3Normal = versor(m_v3Normal);	
	a_hrHitRcd.pMat		= m_pMat;
	
	return true;	
}

bool Parallelogram::shadowHit(const Ray &a_rRay, float a_dMin, float a_dMax) const
{
	Point 	p3HitPlane, p3Offset;
	Vec3	v3Base, v3Origin, v3Offset;
	float	dot1, dot2, t, u1, v1;
	
	dot1 = dot(a_rRay.direction(), m_v3Normal);
	
	if(dot1 < PARALL_EPSILON && dot1 > -PARALL_EPSILON) return false;
	
	v3Base.set(m_p3Base);
	v3Origin.set(a_rRay.origin());
	
	dot2 = dot(m_v3Normal, v3Base);
	t = (dot2 - dot(m_v3Normal, v3Origin)) / dot1;
	
	if(t > a_dMax || t < a_dMin) return false;
	
	p3HitPlane = Ray(a_rRay).getPoint(t);
	v3Offset.set(p3HitPlane - m_p3Base);
	
	u1 = dot(m_v3uNorm, v3Offset) / m_v3u.length();
	if(u1 < 0.f || u1 > 1.f) return false;
	
	v1 = dot(m_v3vNorm, v3Offset) / m_v3v.length();
	if(v1 < 0.f || v1 > 1.f) return false;
	
	return true;
}

bool Parallelogram::getRandomPoint(const Point &p3ViewPoint, CRandomMersenne *rngGen, Point &p3LPoint) const
{
	Point p3U, p3V;
	
	p3U.set(m_v3u);
	p3V.set(m_v3v);
	
	p3LPoint = Point(m_p3Base + rngGen->Random() * p3U + rngGen->Random() * p3V);
	return true;
}
