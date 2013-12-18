#include "aabb/aabb.h"

void AABB::set(const Point &a_p3Min, const Point &a_p3Max)
{
	m_p3Min.e[0] = std::min(a_p3Min.x(), a_p3Max.x());
	m_p3Min.e[1] = std::min(a_p3Min.y(), a_p3Max.y());
	m_p3Min.e[2] = std::min(a_p3Min.z(), a_p3Max.z());
	
	m_p3Max.e[0] = std::max(a_p3Min.x(), a_p3Max.x());
	m_p3Max.e[1] = std::max(a_p3Min.y(), a_p3Max.y());
	m_p3Max.e[2] = std::max(a_p3Min.z(), a_p3Max.z());
}

bool AABB::overlaps(const AABB &bb)
{
	bool x, y, z;
	
	x = (m_p3Max.x() >= bb.m_p3Min.x()) && (m_p3Min.x() <= bb.m_p3Max.x());
	y = (m_p3Max.y() >= bb.m_p3Min.y()) && (m_p3Min.y() <= bb.m_p3Max.y());
	z = (m_p3Max.z() >= bb.m_p3Min.z()) && (m_p3Min.z() <= bb.m_p3Max.z());
	
	return (x && y && z);
}	


bool AABB::inside(const Point &p)
{
	if((p.x() >= m_p3Min.x()) && (p.x() <= m_p3Max.x()))
		if((p.y() >= m_p3Min.y()) && (p.y() <= m_p3Max.y()))
			if((p.z() >= m_p3Min.z()) && (p.z() <= m_p3Max.z()))
				return true;
	
	return false;
}

AABB surround(const AABB &bb1, const AABB &bb2)
{
	AABB bbTemp;
	
	bbTemp.m_p3Min.e[0] = std::min(bb1.m_p3Min.x(), bb2.m_p3Min.x());
	bbTemp.m_p3Min.e[1] = std::min(bb1.m_p3Min.y(), bb2.m_p3Min.y());
	bbTemp.m_p3Min.e[2] = std::min(bb1.m_p3Min.z(), bb2.m_p3Min.z());
	
	bbTemp.m_p3Max.e[0] = std::max(bb1.m_p3Max.x(), bb2.m_p3Max.x());
	bbTemp.m_p3Max.e[1] = std::max(bb1.m_p3Max.y(), bb2.m_p3Max.y());
	bbTemp.m_p3Max.e[2] = std::max(bb1.m_p3Max.z(), bb2.m_p3Max.z());
	
	return bbTemp;
}

AABB surround(const AABB &bb, const Point &p3Point)
{
	AABB bbTemp;
	
	bbTemp.m_p3Min.e[0] = std::min(bb.m_p3Min.x(), p3Point.x());
	bbTemp.m_p3Min.e[1] = std::min(bb.m_p3Min.y(), p3Point.y());
	bbTemp.m_p3Min.e[2] = std::min(bb.m_p3Min.z(), p3Point.z());
	
	bbTemp.m_p3Max.e[0] = std::max(bb.m_p3Max.x(), p3Point.x());
	bbTemp.m_p3Max.e[1] = std::max(bb.m_p3Max.y(), p3Point.y());
	bbTemp.m_p3Max.e[2] = std::max(bb.m_p3Max.z(), p3Point.z());
	
	return bbTemp;
}	
