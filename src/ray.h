#ifndef __RAY_H__
#define __RAY_H__ 1

#include <iostream>

#include "vec3.h"
#include "point.h"
#include "material/material.h"

/**
 * Estructura para almacenar la información de una interseccion.
 */
struct HitRecord 
{
	float 		dDist;		///< Distancia desde el origen a la interseccion.
	Vec3		v3Normal;	///< Normal en el punto de interseccion.
	Material	*pMat;		///< Material del objeto intersectado.
};

/**
 * Clase para el manejo de rayos.
 * 
 * Almacenará los siguientes datos:
 */
class Ray
{
	public:
		Ray() { m_p3Orig = Point(); m_v3Dir = Vec3(); }
		Ray(Point a_p3Orig, Vec3 a_v3Dir)
		{
			m_p3Orig	= a_p3Orig;
			m_v3Dir 	= a_v3Dir;
					
			this->refreshInv();
		}
		Point origin() const { return m_p3Orig; }
		Vec3 direction() const { return m_v3Dir; }
		Vec3 inv_dir() const { return m_v3InvDir; }
		
		void set(const Point &p, const Vec3 &v)
		{
			m_p3Orig	= p;
			m_v3Dir 	= v;
					
			this->refreshInv();
		}
		
		/**
		 * @brief Calcula el punto resultante a la distancia correspondiente.
		 * @param dDist Distancia desde el origen del rayo.
		 * @return Punto resultante.
		 */
		Point getPoint(const float dDist);
		
		/**
		 * @brief Actualiza la informacion de la dirección inversa y de
		 * 		los signos de los componentes.
		 */	
		void refreshInv();		
		
		friend std::ostream& operator<<(std::ostream &os, const Ray &r);
		
		Point 	m_p3Orig; 		///< Origen del rayo.
		Vec3 	m_v3Dir,		///< Direccion.
				m_v3InvDir;		///< Inversa de la dirección.
		int 	m_nSign[3];		///< Signo de los componentes de la direccion invertida.
};

#endif // __RAY_H__
