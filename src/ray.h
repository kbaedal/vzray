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
	double 		dist;		///< Distancia desde el origen a la interseccion.
	Vec3		normal;	    ///< Normal en el punto de interseccion.
	Material	*material;	///< Material del objeto intersectado.
};

/**
 * \class Clase para el manejo de rayos.
 *
 * Almacenará los siguientes datos:
 */
class Ray
{
	public:
		Ray() { orig = Point(0.0f); dir = Vec3(0.0f); }
		Ray(Point a_orig, Vec3 a_dir)
		{
			orig    = a_orig;
			dir 	= a_dir;

			refresh_inv();
		}
		Point origin() const { return orig; }
		Vec3 direction() const { return dir; }
		Vec3 inv_dir() const { return i_dir; }

		void set(const Point &p, const Vec3 &v)
		{
			orig    = p;
			dir 	= v;

			refresh_inv();
		}

		/**
		 * \brief Calcula el punto resultante a la distancia correspondiente.
		 *
		 * \param dist Distancia desde el origen del rayo.
		 * \return Punto resultante.
		 */
		Point get_point(const double dist) const;

		/**
		 * \brief Actualiza la informacion de la dirección inversa y de
		 * 		los signos de los componentes.
		 */
		void refresh_inv();

		friend std::ostream& operator<<(std::ostream &os, const Ray &r);

		Point 	orig; 		///< Origen del rayo.
		Vec3 	dir,		///< Direccion.
				i_dir;	    ///< Inversa de la dirección.
		int 	sign[3];	///< Signo de los componentes de la direccion invertida.
};

#endif // __RAY_H__
