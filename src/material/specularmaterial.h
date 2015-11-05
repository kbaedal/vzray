#ifndef __SPECULARMATERIAL_H__
#define __SPECULARMATERIAL_H__ 1

#include "material/material.h"
#include "rgb.h"
#include "vec3.h"
#include "textures/texture.h"
#include "randomc/randomc.h"

/**
 * \class Definición de material con propiedades reflexivas (un espejo, metal
 * pulido, etc.)
 */
class SpecularMaterial : public Material
{
	public:
		SpecularMaterial(Texture *nueva_t) { texture = nueva_t; }
		~SpecularMaterial() {}

		virtual bool is_specular() { return true; }

		virtual RGB radiance();
		virtual RGB ambient() { return RGB(0.0f, 0.0f, 0.0f); }

		/**
		 * \brief Aplicando las leyes de la reflexión, obtendremos la dirección.
		 */
		virtual Vec3 out_direction(Vec3 const &in, Vec3 const &norm, double &brdf, RGB &color, CRandomMersenne *rng)
		{
			brdf    = 1.0f;
			color   = radiance();
			return reflect(in, norm);
		}
};

#endif // __SPECULARMATERIAL_H__
