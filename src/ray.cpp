#include "ray.h"

Point Ray::getPoint(const float dDist)
{
	return Point(m_p3Orig + dDist * Point(m_v3Dir.x(), m_v3Dir.y(), m_v3Dir.z()));
}

void Ray::refreshInv()
{
	// Inverted direction and signs, for ray-box tests
	m_v3InvDir.set(1.f/m_v3Dir.x(), 1.f/m_v3Dir.y(), 1.f/m_v3Dir.z());
	
	// Sign could be 0 or 1 (int values for false and true)
	m_nSign[0] = (m_v3InvDir.x() < 0.f);
	m_nSign[1] = (m_v3InvDir.y() < 0.f);
	m_nSign[2] = (m_v3InvDir.z() < 0.f);
}

std::ostream& operator<<(std::ostream &os, const Ray &r)
{
	os << "(" << r.origin() << ") + t(" << r.direction() << ")";
	
	return os;
}
