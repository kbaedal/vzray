#include "camera/pinhole.h"

void Pinhole::set(Point a_p3Pinhole, Vec3 a_v3Dir, Vec3 a_v3Up,
			float a_dDist, float a_u0, float a_v0, float a_u1, float a_v1, float a_dPush)
{
	Point	p3U, p3V, p3W;	// Auxiliares para calculos.
	
	m_p3Pinhole = a_p3Pinhole;	// eye
	m_v3Dir		= a_v3Dir;		// gaze
	m_v3Up		= a_v3Up;		// up
	m_dDist		= a_dDist;		// s
	m_u0		= a_u0;
	m_v0		= a_v0;
	m_u1		= a_u1;
	m_v1		= a_v1;
	
	m_uvw.initFromWV(-m_v3Dir, m_v3Up);
	
	p3U.set(m_uvw.u());
	p3V.set(m_uvw.v());
	p3W.set(m_uvw.w());
	
	//m_p3Corner = m_p3Pinhole + m_u0 * m_uvw.u() + m_v0 * m_uvw.v() - m_dDist * m_uvw.w();
	m_p3Corner = m_p3Pinhole + m_u0 * p3U + m_v0 * p3V - m_dDist * p3W;
	
	m_v3XDir = (m_u1 - m_u0) * m_uvw.u(); 
	m_v3YDir = (m_v1 - m_v0) * m_uvw.v();
	
	m_dPush = a_dPush;
}

Ray Pinhole::getRay(float x, float y, float sx, float sy)
{
	Point 	p3ImgPlane, p3XDir, p3YDir, p3PushDir;
	Vec3	v3Dir;
	
	p3XDir.set(m_v3XDir);
	p3YDir.set(m_v3YDir);
	p3ImgPlane = m_p3Corner + x * p3XDir + y * p3YDir;
	
	v3Dir.set(p3ImgPlane - m_p3Pinhole);
	v3Dir.normalize();
	p3PushDir.set(v3Dir);
		
	return Ray(m_p3Pinhole + m_dPush * p3PushDir, v3Dir);
}
