#ifndef __IMAGE_H__
#define __IMAGE_H__ 1

#include <iostream>
#include "rgb.h"

/**
 * Clase para el manejo de la imagen generada por el raytracer.
 *
 * Se encargará tanto de almacenar los resultados del proceso como
 * de guardar la imagen en disco en cualquiera de los formatos
 * soportados. 
 */
class Image
{
	public:
		Image();
		Image(int a_nWidth, int a_nHeight);
		Image(int a_nWidth, int a_nHeight, const RGB &a_rgbColor);
		bool set(int a_nWidth, int a_nHeight, const RGB &a_rgbColor);
		bool get(int a_nWidth, int a_nHeight, RGB &a_rgbColor);
		
		/**
		 * @brief Corrige la imagen con el valor gamma indicado.
		 * @param dGamma Valor de gamma.
		 */
		void gammaCorrect(float dGamma);
		
		/**
		 * @brief Guarda la imagen en formato PPM.
		 * @param os Stream en modo binario del fichero.
		 */
		void savePPM(std::ostream &os);
		
	private:
		RGB **m_rgbImage;
		int	m_nWidth;
		int m_nHeight;
};

#endif // __IMAGE_H__
