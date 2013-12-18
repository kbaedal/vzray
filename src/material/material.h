#ifndef __MATERIAL_H__
#define __MATERIAL_H__ 1

#include "rgb.h"
#include "vec3.h"
#include "textures/texture.h"
#include "randomc/randomc.h"

/**
 * Definición de la clase material, que definirá las propiedades
 * de la superficie de un objeto.
 */
class Material
{
	public:
	    virtual ~Material() {};

		/**
		 * Devuelve true si el material emite luz.
		 */
		virtual bool isLight() { return false; }

		/**
		 * Devuelve true si el material tiene propiedades reflexivas.
		 */
		virtual bool isSpecular() { return false; }

		/**
		 * Devuelve true si el material tiene propiedades transmisivas.
		 */
		virtual bool isTransmissive() { return false; }

		/**
		 * Calcula el color de una superficie cuando esta es iluminada.
		 */
		virtual RGB radiance() { return RGB(0.f, 0.f, 0.f); }

		/**
		 * Calcula el color de una superficie cuando esta no es iluminada.
		 */
		virtual RGB ambient() { return RGB(0.f, 0.f, 0.f); }

		/**
		 * Calcula la luz emitida por la superficie.
		 */
		virtual RGB emittance() { return RGB(0.f, 0.f, 0.f); }

		/**
		 * Calcula una direccion con los valores indicados y las caracterísiticas
		 * del material.
		 */
		virtual Vec3 outDirection(Vec3 const &v3In, Vec3 const &v3Norm, float &dBRDF, RGB &rgbColor, CRandomMersenne *rng)
		{
			return Vec3(0.f, 0.f, 0.f);
		}

		void setTexture(Texture *a_pTexture) { m_pTexture = a_pTexture; }

		Texture *m_pTexture;
};

#endif
