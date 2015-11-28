#ifndef __IMAGE_H__
#define __IMAGE_H__ 1

#include <iostream>
#include "rgb.h"

/**
 * \class Clase para el manejo de la imagen generada por el raytracer.
 *
 * Se encargará tanto de almacenar los resultados del proceso como
 * de guardar la imagen en disco en cualquiera de los formatos
 * soportados.
 */
class Image
{
	public:
		~Image();
		Image(int a_width, int a_height);
		Image(int a_width, int a_height, const RGB &a_color);
		bool set(int a_width, int a_height, const RGB &a_color);
		bool set(int a_width, int a_height, const Contrib &a_contrib);
		bool get(int a_width, int a_height, RGB &a_color);
		bool get(int a_width, int a_height, Contrib &a_contrib);

		/**
		 * \brief Corrige la imagen con el valor gamma indicado, para todos los componentes.
		 *
		 * \param dGamma Valor de gamma.
		 */
		void gamma_correct(double gamma);

		/**
		 * \brief Guarda la imagen del componente indicado en formato PPM.
		 *
		 * \param os Stream en modo binario del fichero.
		 * \param type Componente a guardar. Si 0, imagen completa.
		 */
		void save_ppm(std::ostream &os, int type = 0);

		/**
		 * \brief Compone la imagen final sumando las contribuciones.
		 */
        void create_final_img();

	private:
		Contrib **pixels;
		int	width;
		int height;
};

#endif // __IMAGE_H__

