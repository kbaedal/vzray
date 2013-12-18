#ifndef __LIGHTMATERIAL_H__
#define __LIGHTMATERIAL_H__ 1

#include "material/material.h"
#include "rgb.h"
#include "vec3.h"
#include "textures/texture.h"
#include "randomc/randomc.h"

/**
 * Define las propiedades de un material que emite luz.
 */
class LightMaterial : public Material
{	
	public:
		LightMaterial(Texture *a_pTexture) { m_pTexture = a_pTexture; }
		~LightMaterial() {}
		
		virtual bool isLight() { return true; }
				
		virtual RGB radiance();
		virtual RGB emittance();
		virtual RGB ambient() { return RGB(0.0, 0.0, 0.0); }
		virtual Vec3 outDirection(Vec3 const &v3In, Vec3 const &v3Norm, float &dBRDF, RGB &rgbColor, CRandomMersenne *rng)
		{ 
			dBRDF = 1;
			rgbColor = this->radiance();
			return versor(randomDir(v3Norm, rng->Random(), rng->Random()));;
		}		
};

#endif // __DIFFUSEMATERIAL_H__
