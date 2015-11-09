#include "material/dielectricmaterial.h"
#include "vec2.h"
#include "vec3.h"

#include <stdio.h>

RGB DielectricMaterial::radiance()
{
	Vec2 v2;
	Vec3 v3;
	return texture->valor(v2, v3);
}

Vec3 DielectricMaterial::out_direction(Vec3 const &in, Vec3 const &norm, double &brdf, RGB &color, CRandomMersenne *rng)
{
	bool 	going_in;   // Ray going in or going out?
	Vec3 	t_norm;		// Reversed normal, if needed
	float	n1,
			n2;			// Refraction index
	float	cos_t,
			cos_t2;     // Aux for TIR calculation

	cos_t = dot(in, norm);
	if(cos_t < 0.0f) {
		going_in    = true; // Incoming ray
		t_norm      = norm;
		n1          = 1.0f;
		n2	        = ior;
		cos_t       = -cos_t;

	}
	else {
		going_in    = false; // Outgoing ray, reverse normal, and refraction index
		t_norm      = -norm;
		n1          = ior;
		n2	        = 1.0f;

		double t = -(dot(in, t_norm));
		cos_t2 = 1.0f - (ior * ior) * (1.0f - t * t);
	}

	color = radiance();

	// If Outgoing ray and Total internal reflection, reflect.
	if(!going_in && (cos_t2 < 0.0f))
	{
		brdf = 1.0f;
		return reflect(in, t_norm);
	}

	// Otherwise, calculate fresnel term.
	double n = n1/n2;
	double r0 = ((n - 1) * (n - 1)) / ((n + 1) * (n + 1));
	double c = 1 - cos_t;

	double re = r0 + (1 - r0) * c * c * c * c * c;
	double pr = (re + 0.5) / 2.0;
	double rg = rng->Random();


	if(rg > pr) { // Transmit
		brdf = (1.0 - re) / (1.0 - pr);
		return refract(in, t_norm, n1, n2);
	} else { // Refract
		brdf = re / pr;
		return reflect(in, t_norm);
	}
}

bool DielectricMaterial::is_TIR(Vec3 const &in, Vec3 const &norm)
{
	double cos_t2;			// Aux for TIR calculation

	if(dot(in, norm) < 0.0f) {
		return false;
	}
	else {
		double t = -(dot(in, -norm));
		cos_t2 = 1.0f - (ior * ior) * (1.0f - t * t);

		if(cos_t2 < 0.0f)
		{
			return true;
		}
	}
	return false;
}

Vec3 DielectricMaterial::reflect_dir(Vec3 const &in, Vec3 const &norm, double &brdf, RGB &color)
{
	double n1, n2, cos_t;

	cos_t = dot(in, norm);
	if(cos_t < 0.0f) { // Incoming ray
		n1      = 1.0f;
		n2	    = ior;
		cos_t    = cos_t;
	}
	else { // Outgoing ray
		n1      = ior;
		n2      = 1.0f;
	}

	double n = n1/n2;
	double r0 = ((n - 1) * (n - 1)) / ((n + 1) * (n + 1));
	double c = 1 - cos_t;

	double re = r0 + (1 - r0) * c * c * c * c * c;
	double pr = (re + 0.5) / 2.0;

	brdf = re / pr;
	color = radiance();

	if(dot(in, norm) < 0.0f) { // Ray going in
		return reflect(in, norm);
	} else { // Ray going out
		return reflect(in, -norm);
	}
}

Vec3 DielectricMaterial::refract_dir(Vec3 const &in, Vec3 const &norm, double &brdf, RGB &color)
{
	double n1, n2, cos_t;

	cos_t = dot(in, norm);
	if(cos_t < 0.0f) { // Incoming ray
		n1      = 1.0f;
		n2	    = ior;
		cos_t    = -cos_t;
	}
	else { // Outgoing ray
		n1      = ior;
		n2      = 1.0f;
	}

	double n = n1/n2;
	double r0 = ((n - 1) * (n - 1)) / ((n + 1) * (n + 1));
	double c = 1 - cos_t;

	double re = r0 + (1 - r0) * c * c * c * c * c;
	double pr = (re + 0.5) / 2.0;

	//dBRDF = (1.0 - Re) / (1.0 - Pr);
	brdf = 1 - (re / pr);
	color = radiance();

	if(dot(in, norm) < 0.0f) { // Ray going in
		return refract(in, norm, 1.0, ior);
	} else { // Ray going out
		return refract(in, -norm, ior, 1.0);
	}
}

double DielectricMaterial::get_reflectance(Vec3 const &in, Vec3 const &norm)
{
	return 0.f;
}
