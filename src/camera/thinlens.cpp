#include "camera/thinlens.h"

void ThinLens::set(Point a_p3Center, Vec3 a_v3Dir, Vec3 a_v3Up, float a_dAperture, 
			float a_dDist, float a_u0, float a_v0, float a_u1, float a_v1)
{
	Point	p3U, p3V, p3W;	// Auxiliares para calculos.
	
	m_p3Center 		= a_p3Center;	// eye
	m_v3Dir			= a_v3Dir;		// gaze
	m_v3Up			= a_v3Up;		// up
	m_dDist			= a_dDist;		// s
	m_u0			= a_u0;
	m_v0			= a_v0;
	m_u1			= a_u1;
	m_v1			= a_v1;
	m_dLensRadius	= a_dAperture / 2.0f;
	
	m_uvw.initFromWV(-m_v3Dir, m_v3Up);
	
	p3U.set(m_uvw.u());
	p3V.set(m_uvw.v());
	p3W.set(m_uvw.w());
	
	m_p3Corner = m_p3Center + m_u0 * p3U + m_v0 * p3V - m_dDist * p3W;
	
	m_v3XDir = (m_u1 - m_u0) * m_uvw.u(); 
	m_v3YDir = (m_v1 - m_v0) * m_uvw.v();
}

Ray ThinLens::getRay(float x, float y, float sx, float sy)
{
	Point 	p3LensPoint, p3ImgPlane,
			p3U, p3V, p3X, p3Y;
	Vec3	v3Dir;
	
	// Auxiliares para los calculos
	p3U.set(m_uvw.u());
	p3V.set(m_uvw.v());
	
	p3X.set(m_v3XDir);
	p3Y.set(m_v3YDir);
	
	// Calculamos un punto en la lente. Será el punto baso del rayo.
	p3LensPoint = m_p3Center + 2.0f * (sx - 0.5f) * m_dLensRadius * p3U + 
		2.0f * (sy - 0.5f) * m_dLensRadius * p3V;
	
	// Calculamos el punto correcto en el plano de la imagen.
	p3ImgPlane = m_p3Corner + x * p3X + y * p3Y;
	
	// Obtenemos el vector del rayo.
	v3Dir.set(p3ImgPlane - p3LensPoint);
	v3Dir.normalize();
	
	return Ray(p3LensPoint, v3Dir);
}
