#ifndef __DIELECTRICMATERIAL_H__
#define __DIELECTRICMATERIAL_H__ 1

#include "material/material.h"
#include "rgb.h"
#include "vec3.h"
#include "textures/texture.h"
#include "randomc/randomc.h"

/**
 * Define las propiedades de un material transmisivo (cristal, agua)
 */
class DielectricMaterial : public Material
{	
	public:
		DielectricMaterial(Texture *a_pTexture, float a_dIOR) 
		{ 
			m_pTexture		= a_pTexture; 
			m_dIOR 			= a_dIOR;
			m_dReflectance 	= 1.0f;
			m_MyRNG.RandomInit(12321);
		}
		~DielectricMaterial() {}
				
		virtual bool isTransmissive() { return true; }
		
		virtual RGB radiance();
		virtual RGB ambient() { return RGB(0.0, 0.0, 0.0); }
		virtual Vec3 outDirection(Vec3 const &v3In, Vec3 const &v3Norm, float &dBRDF, RGB &rgbColor, CRandomMersenne *rng);
						
		float			m_dIOR;
		float 			m_dReflectance;
		CRandomMother	m_MyRNG;	
		
		/**
		 * Devuelve true si se produce Reflexión Interna Total.
		 */
		bool isTIR(Vec3 const &v3In, Vec3 const &v3Norm);
		
		/**
		 * Aplicando las leyes de Snell, obtendremos la direccion de reflexión.
		 */
		Vec3 reflectDir(Vec3 const &v3In, Vec3 const &v3Norm, float &dBRDF, RGB &rgbColor);
		
		/**
		 * Aplicando las leyes de Snell, obtendremos la direccion de refracción.
		 */
		Vec3 refractDir(Vec3 const &v3In, Vec3 const &v3Norm, float &dBRDF, RGB &rgbColor);
		
		/**
		 * Aplicando las leyes de Snell, obtendremos la reflectividad de la superficie.
		 */
		float getReflectance(Vec3 const &v3In, Vec3 const &v3Norm);
};

#endif // __DIELECTRICMATERIAL_H__
