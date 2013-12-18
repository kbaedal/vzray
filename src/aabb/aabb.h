#ifndef __AABB_H__
#define __AABB_H__

#include <cmath>

#include "point.h"

/**
 * Clase para el manejo de Axis Aligned Bounding Boxes.
 */
class AABB
{
	public:
		AABB()
		{
			// Bounding Box degenerada (min > max) para asegurarnos
			// de que las operaciones sobre BB vacías como esta se
			// realizan correctamente.
			m_p3Min.set(INFINITY, INFINITY, INFINITY);
			m_p3Max.set(-INFINITY, -INFINITY, -INFINITY);
		}
		AABB(const Point &a_p3Min, const Point &a_p3Max)
		{
			this->set(a_p3Min, a_p3Max);
		}
		AABB(const Point &p3Point)
		{
			m_p3Min.set(p3Point);
			m_p3Max.set(p3Point);
		}
				
		void set(const Point &a_p3Min, const Point &a_p3Max);
		
		/**
		 * Calcula si otra AABB intersecta en algun punto con esta.
		 */
		bool overlaps(const AABB &bb);
		
		/**
		 * Calcula si un punto está dentro del espacio delimitado
		 * por la AABB.
		 */
		bool inside(const Point &p);
		
		/**
		 * Calcula una nueva AABB que contenga a las dos AABB pasadas
		 * como argumentos.
		 * @param bb1 Una AABB.
		 * @param bb2 Otra AABB.
		 * @return Una AABB que contiene a ambas.
		 */		 
		friend AABB surround(const AABB &bb1, const AABB &bb2);
		
		/**
		 * Calcula una nueva AABB que contenga a la AABB y al punto
		 * pasados como argumentos.
		 * @param bb Una AABB.
		 * @param v3Point El punto.
		 * @return Una AABB que contiene a ambos.
		 */		 
		friend AABB surround(const AABB &bb, const Point &p3Point);
		
		Point 	m_p3Min,	///< Punto mínimo de la AABB.
				m_p3Max;	///< Punto máximo de la AABB.
		
};

#endif // __AABB_H__
