#ifndef __AABB_H__
#define __AABB_H__

#include <cmath>

#include "vec3.h"
#include "point.h"
#include "ray.h"

/**
 * \class Clase para el manejo de Axis Aligned Bounding Boxes.
 */
class AABB
{
	public:
		AABB()
		{
			// Bounding Box degenerada (min > max) para asegurarnos
			// de que las operaciones sobre BB vacías como esta se
			// realizan correctamente.
			minimo.set(INFINITY, INFINITY, INFINITY);
			maximo.set(-INFINITY, -INFINITY, -INFINITY);
		}
		AABB(const Point &a_min, const Point &a_max) : minimo(a_min), maximo(a_max) {};
		AABB(const Point &p): minimo(p), maximo(p) {};
		AABB(const AABB &aabb) : minimo(aabb.minimo), maximo(aabb.maximo) {};

		void set(const Point &a_min, const Point &a_max);

		/**
		 * \brief Calcula si otra AABB intersecta en algun punto con esta.
		 *
		 * \param bb AABB a comprobar.
		 */
		bool overlaps(const AABB &b) const;

		/**
		 * \brief Calcula si un punto está dentro del espacio delimitado
		 * por la AABB.
		 *
		 * \param p Punto a comprobar.
		 */
		bool inside(const Point &p) const;

		/**
		 * \brief Comprueba si un rayo interesecta la AABB.
		 *
		 * \param r Rayo a comprobar.
		 * \param min_dist Distancia mínima a comprobar.
		 * \param max_dist Distancia máxima a comprobar.
		 */
		bool hit(const Ray &r, double min_dist, double max_dist) const;

		/**
		 * \brief Devuelve el punto central de la AABB, el centroide.
		 *
		 */
		Vec3 centroid() const
		{
		    return ((minimo + maximo) / 2.0f);
		};

		/**
		 * \brief Calcula una nueva AABB que contenga a las dos AABB pasadas
		 * como argumentos.
		 *
		 * \param bb1 Una AABB.
		 * \param bb2 Otra AABB.
		 * \return Una AABB que contiene a ambas.
		 */
		friend AABB surround(const AABB &b1, const AABB &b2);

		/**
		 * \brief Calcula una nueva AABB que contenga a la AABB y al punto
		 * pasados como argumentos.
		 *
		 * \param bb Una AABB.
		 * \param v3Point El punto.
		 * \return Una AABB que contiene a ambos.
		 */
		friend AABB surround(const AABB &b, const Point &p);

		friend std::ostream &operator<<(std::ostream &os, const AABB &ab);

		Point 	minimo,     ///< Punto mínimo de la AABB.
				maximo;	    ///< Punto máximo de la AABB.

};

#endif // __AABB_H__
