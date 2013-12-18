#ifndef __PINHOLE_H__
#define __PINHOLE_H__

#include "camera.h"

#include "vec3.h"
#include "point.h"
#include "onb.h"
#include "ray.h"

/**
 * Definicion de una camara estenopeica o pinhole.
 */
class Pinhole : public Camera {
	public:
		Pinhole() {}
		Pinhole(Point a_p3Pinhole, Vec3 a_v3Dir, Vec3 a_v3Up,
			float a_dDist, float a_u0, float a_v0, float a_u1, float a_v1, float a_dPush = 0.f)
		{
			this->set(a_p3Pinhole, a_v3Dir, a_v3Up, a_dDist, a_u0, a_v0, a_u1, a_v1, a_dPush);
		}
		
		void set(Point a_p3Pinhole, Vec3 a_v3Dir, Vec3 a_v3Up,
			float a_dDist, float a_u0, float a_v0, float a_u1, float a_v1, float a_dPush = 0.f);
			
		Ray getRay(float x, float y, float sx, float sy);
		
	private:
		Point	m_p3Pinhole,	///< Punto en el que está situada la camara.
				m_p3Corner; 	///< Esquina inferior de la imagen
				
		Vec3	m_v3Dir, 		///< Dirección en la que mira.
				m_v3Up, 		///< Vector que define qué es arriba.
				m_v3XDir, 		///< Vector x con el que avanzaremos por el	plano de la imagen.
				m_v3YDir;		///< Vector y para lo mismo.
				
		ONB 	m_uvw;			///< Base ortonormal de la camara.
		
		float 	m_dDist, 		///< Distancia al plano de la imagen.
				m_u0,			///< Coordenada u0 del plano de la imagen.
				m_v0, 			///< Coordenada v0 del plano de la imagen.
				m_u1, 			///< Coordenada u1 del plano de la imagen.
				m_v1;			///< Coordenada v1 del plano de la imagen.
		
		float 	m_dPush; // Trick for test scenes.
};

#endif // __PINHOLE_H__
