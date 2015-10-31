#ifndef __DIFFUSEMATERIAL_H__
#define __DIFFUSEMATERIAL_H__ 1

#include "material/material.h"
#include "rgb.h"
#include "vec3.h"
#include "textures/texture.h"
#include "randomc/randomc.h"

/**
 * \class Define las propiedades de un material lambertiano ideal.
 */
class DiffuseMaterial : public Material
{
	public:
		DiffuseMaterial(Texture *nueva_t) { texture = nueva_t; }
		~DiffuseMaterial() {}

		virtual RGB radiance();
		virtual RGB ambient() { return RGB(0.0f, 0.0f, 0.0f); }
		virtual Vec3 out_direction(Vec3 const &in, Vec3 const &norm, float &brdf, RGB &color, CRandomMersenne *a_rng);
};

#endif // __DIFFUSEMATERIAL_H__
