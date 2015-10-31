#ifndef __MATERIAL_H__
#define __MATERIAL_H__ 1

#include "rgb.h"
#include "vec3.h"
#include "textures/texture.h"
#include "randomc/randomc.h"

/**
 * \class Definición de la clase material, que definirá las propiedades
 * de la superficie de un objeto.
 */
class Material
{
	public:
	    virtual ~Material() {};

		/**
		 * \brief Devuelve true si el material emite luz.
		 */
		virtual bool is_light() { return false; }

		/**
		 * \brief Devuelve true si el material tiene propiedades reflexivas.
		 */
		virtual bool is_specular() { return false; }

		/**
		 * \brief Devuelve true si el material tiene propiedades transmisivas.
		 */
		virtual bool is_transmissive() { return false; }

		/**
		 * \brief Calcula el color de una superficie cuando esta es iluminada.
		 */
		virtual RGB radiance() { return RGB(0.0f, 0.0f, 0.0f); }

		/**
		 * \brief Calcula el color de una superficie cuando esta no es iluminada.
		 */
		virtual RGB ambient() { return RGB(0.0f, 0.0f, 0.0f); }

		/**
		 * \brief Calcula la luz emitida por la superficie.
		 */
		virtual RGB emittance() { return RGB(0.0f, 0.0f, 0.0f); }

		/**
		 * \brief Calcula una direccion con los valores indicados y las caracterísiticas
		 * del material.
		 */
		virtual Vec3 out_direction(Vec3 const &in, Vec3 const &norm, float &brdf, RGB &color, CRandomMersenne *rng)
		{
			return Vec3(0.0f, 0.0f, 0.0f);
		}

		void set_texture(Texture *a_texture) { texture = a_texture; }

		Texture *texture;
};

#endif
