#ifndef __SPECULARMATERIAL_H__
#define __SPECULARMATERIAL_H__ 1

#include "material/material.h"
#include "rgb.h"
#include "vec3.h"
#include "textures/texture.h"
#include "randomc/randomc.h"

/**
 * Definición de material con propiedades reflexivas (un espejo, metal
 * pulido, etc.)
 */
class SpecularMaterial : public Material
{	
	public:
		SpecularMaterial(Texture *a_pTexture) { m_pTexture = a_pTexture; }
		~SpecularMaterial() {}
		
		virtual bool isSpecular() { return true; }
		
		virtual RGB radiance();
		virtual RGB ambient() { return RGB(0.0, 0.0, 0.0); }
		
		/**
		 * Aplicando las leyes de la reflexión, obtendremos la dirección.
		 */
		virtual Vec3 outDirection(Vec3 const &v3In, Vec3 const &v3Norm, float &dBRDF, RGB &rgbColor, CRandomMersenne *rng)
		{
			dBRDF = 1.0;
			rgbColor = this->radiance();
			return reflect(v3In, v3Norm);
		}		
};

#endif // __SPECULARMATERIAL_H__
