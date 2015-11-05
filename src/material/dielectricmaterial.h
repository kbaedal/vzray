#ifndef __DIELECTRICMATERIAL_H__
#define __DIELECTRICMATERIAL_H__ 1

#include "material/material.h"
#include "rgb.h"
#include "vec3.h"
#include "textures/texture.h"
#include "randomc/randomc.h"

/**
 * \class Define las propiedades de un material transmisivo (cristal, agua)
 */
class DielectricMaterial : public Material
{
	public:
		DielectricMaterial(Texture *a_texture, double a_ior)
		{
			texture		    = a_texture;
			ior 			= a_ior;
			reflectance 	= 1.0f;

			rng.RandomInit(12321);
		}
		~DielectricMaterial() {}

		virtual bool is_transmissive() { return true; }

		virtual RGB radiance();
		virtual RGB ambient() { return RGB(0.0f, 0.0f, 0.0f); }
		virtual Vec3 out_direction(Vec3 const &in, Vec3 const &norm, double &brdf, RGB &color, CRandomMersenne *rng);

		float			ior;
		double 			reflectance;
		CRandomMother	rng;

		/**
		 * \brief Devuelve true si se produce Reflexión Interna Total.
		 */
		bool is_TIR(Vec3 const &in, Vec3 const &norm);

		/**
		 * \brief Aplicando las leyes de Snell, obtendremos la direccion de reflexión.
		 */
		Vec3 reflect_dir(Vec3 const &in, Vec3 const &norm, double &brdf, RGB &color);

		/**
		 * \brief Aplicando las leyes de Snell, obtendremos la direccion de refracción.
		 */
		Vec3 refract_dir(Vec3 const &in, Vec3 const &norm, double &brdf, RGB &color);

		/**
		 * \brief Aplicando las leyes de Snell, obtendremos la reflectividad de la superficie.
		 */
		double get_reflectance(Vec3 const &in, Vec3 const &norm);
};

#endif // __DIELECTRICMATERIAL_H__
