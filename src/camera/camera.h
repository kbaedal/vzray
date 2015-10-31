#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "ray.h"

/**
 * \class Clase abstracta de la que derivarán las clases que manejarán los
 * diferentes tipos de cámaras.
 */
class Camera {
	public:
	    virtual ~Camera() {};

		/**
		 * Devuelve un rayo para el pixel indicado.
		 * @param x Coordenada x del pixel.
		 * @param y Coordenada y del pixel.
		 * @param sx Muestra del pixel, tal que 0 <= sx < 1.
		 * @param sy Muestra del pixel, tal que 0 <= sy < 1.
		 * @return Rayo generado por la camara.
		 */
		virtual Ray get_ray(float x, float y, float sx, float sy) = 0;
};

#endif // __CAMERA_H__
