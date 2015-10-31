#ifndef __LIGHTMATERIAL_H__
#define __LIGHTMATERIAL_H__ 1

#include "material/material.h"
#include "rgb.h"
#include "vec3.h"
#include "textures/texture.h"
#include "randomc/randomc.h"

/**
 * \class Define las propiedades de un material que emite luz.
 */
class LightMaterial : public Material
{
	public:
		LightMaterial(Texture *nueva_t) { texture = nueva_t; }
		~LightMaterial() {}

		virtual bool is_light() { return true; }

		virtual RGB radiance();
		virtual RGB emittance();
		virtual RGB ambient() { return RGB(0.0f, 0.0f, 0.0f); }
		virtual Vec3 out_direction(Vec3 const &in, Vec3 const &norm, float &brdf, RGB &color, CRandomMersenne *rng)
		{
			brdf    = 1.0f;
			color   = radiance();
			return versor(random_dir(norm, rng->Random(), rng->Random()));;
		}
};

#endif // __DIFFUSEMATERIAL_H__
