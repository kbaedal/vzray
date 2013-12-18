#ifndef __DIFFUSEMATERIAL_H__
#define __DIFFUSEMATERIAL_H__ 1

#include "material/material.h"
#include "rgb.h"
#include "vec3.h"
#include "textures/texture.h"
#include "randomc/randomc.h"

/**
 * Define las propiedades de un material lambertiano ideal.
 */
class DiffuseMaterial : public Material
{	
	public:
		DiffuseMaterial(Texture *a_pTexture) { m_pTexture = a_pTexture; }
		~DiffuseMaterial() {}
		
		virtual RGB radiance();
		virtual RGB ambient() { return RGB(0.0, 0.0, 0.0); }
		virtual Vec3 outDirection(Vec3 const &v3In, Vec3 const &v3Norm, float &dBRDF, RGB &rgbColor, CRandomMersenne *rng);
};

#endif // __DIFFUSEMATERIAL_H__
