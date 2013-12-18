#ifndef __THINLENS_H__
#define __THINLENS_H__

#include "vec3.h"
#include "point.h"
#include "onb.h"
#include "ray.h"
#include "camera/camera.h"

/**
 * Definición de una cámara de lente fina (ThinLens Camera)
 */
class ThinLens : public Camera {
	public:
		ThinLens() {}
		ThinLens(Point a_p3Center, Vec3 a_v3Dir, Vec3 a_v3Up, float a_dAperture, 
			float a_dDist, float a_u0, float a_v0, float a_u1, float a_v1)
		{
			this->set(a_p3Center, a_v3Dir, a_v3Up, a_dAperture, a_dDist, a_u0, a_v0, a_u1, a_v1);
		}
		
		void set(Point a_p3Center, Vec3 a_v3Dir, Vec3 a_v3Up, float a_dAperture, 
			float a_dDist, float a_u0, float a_v0, float a_u1, float a_v1);
			
		Ray getRay(float x, float y, float sx, float sy);
	private:
		Point	m_p3Center,
				m_p3Corner;
				 
		Vec3	m_v3Dir, 
				m_v3Up, 				
				m_v3XDir, 
				m_v3YDir;

		ONB 	m_uvw;
		
		float	m_dLensRadius,	///< Radio de la lente.
				m_dDist,
				m_u0,
				m_v0,
				m_u1,
				m_v1;
};

#endif // __THINLENS_H__
