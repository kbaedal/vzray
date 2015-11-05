#ifndef __PINHOLE_H__
#define __PINHOLE_H__

#include "camera.h"

#include "vec3.h"
#include "point.h"
#include "onb.h"
#include "ray.h"

/**
 * \class Definicion de una camara estenopeica o pinhole.
 */
class Pinhole : public Camera {
	public:
		Pinhole() {}
		Pinhole(Point a_pos, Vec3 a_dir, Vec3 a_up,
			double a_dist, double a_u0, double a_v0, double a_u1, double a_v1, double a_push = 0.0f)
		{
			set(a_pos, a_dir, a_up, a_dist, a_u0, a_v0, a_u1, a_v1, a_push);
		}

		void set(Point a_pos, Vec3 a_dir, Vec3 a_up,
			double a_dist, double a_u0, double a_v0, double a_u1, double a_v1, double a_push = 0.0f);

		Ray get_ray(double x, double y, double sx, double sy) final;

	private:
		Point	pos,	    ///< Punto en el que está situada la camara.
				corner; 	///< Esquina inferior de la imagen

		Vec3	dir, 		///< Dirección en la que mira.
				up, 		///< Vector que define qué es arriba.
				x_dir, 		///< Vector x con el que avanzaremos por el	plano de la imagen.
				y_dir;		///< Vector y para lo mismo.

		ONB 	uvw;		///< Base ortonormal de la camara.

		double 	dist, 		///< Distancia al plano de la imagen.
				u0,			///< Coordenada u0 del plano de la imagen.
				v0, 		///< Coordenada v0 del plano de la imagen.
				u1, 		///< Coordenada u1 del plano de la imagen.
				v1;			///< Coordenada v1 del plano de la imagen.

		double 	push;       // Trick for test scenes.
};

#endif // __PINHOLE_H__
