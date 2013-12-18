#include "material/dielectricmaterial.h"
#include "vec2.h"
#include "vec3.h"

#include <stdio.h>

RGB DielectricMaterial::radiance()
{
	Vec2 unVec2;
	Vec3 unVec3;
	return m_pTexture->valor(unVec2, unVec3);
}

Vec3 DielectricMaterial::outDirection(Vec3 const &v3In, Vec3 const &v3Norm, float &dBRDF, RGB &rgbColor, CRandomMersenne *rng)
{
	bool 	bIn;			// Ray going in or going out?
	Vec3 	v3TempNorm;		// Reversed normal, if needed
	float	dN1,
			dN2;			// Refraction index
	float	dCosT,
			dCosT2;			// Aux for TIR calculation

	dCosT = dot(v3In, v3Norm);
	if(dCosT < 0.0f) {
		bIn = true; // Incoming ray
		v3TempNorm = v3Norm;
		dN1 = 1.0f;
		dN2	= m_dIOR;
		dCosT = -dCosT;

	} else {
		bIn = false; // Outgoing ray, reverse normal, and refraction index
		v3TempNorm = -v3Norm;
		dN1 = m_dIOR;
		dN2	= 1.0f;

		float dTemp = -(dot(v3In, v3TempNorm));
		dCosT2 = 1.0f - (m_dIOR * m_dIOR) * (1.0f - dTemp * dTemp);
	}

	rgbColor = this->radiance();

	// If Outgoing ray and Total internal reflection, reflect.
	if(!bIn && (dCosT2 < 0.0f))
	{
		dBRDF = 1.0f;
		return reflect(v3In, v3TempNorm);
	}

	// Otherwise, calculate fresnel term.
	float dN = dN1/dN2;
	float R0 = ((dN - 1) * (dN - 1)) / ((dN + 1) * (dN + 1));
	float dC = 1 - dCosT;

	float Re = R0 + (1 - R0) * dC * dC * dC * dC * dC;
	float Pr = (Re + 0.5) / 2.0;
	float Rg = rng->Random();


	if(Rg > Pr) { // Transmit
		dBRDF = (1.0 - Re) / (1.0 - Pr);
		return refract(v3In, v3TempNorm, dN1, dN2);
	} else { // Refract
		dBRDF = Re / Pr;
		return reflect(v3In, v3TempNorm);
	}
}

bool DielectricMaterial::isTIR(Vec3 const &v3In, Vec3 const &v3Norm)
{
	float dCosT2;			// Aux for TIR calculation

	if(dot(v3In, v3Norm) < 0.0f) {
		return false;
	} else {
		float dTemp = -(dot(v3In, -v3Norm));
		dCosT2 = 1.0f - (m_dIOR * m_dIOR) * (1.0f - dTemp * dTemp);

		if(dCosT2 < 0.0f)
		{
			return true;
		}
	}
	return false;
}

Vec3 DielectricMaterial::reflectDir(Vec3 const &v3In, Vec3 const &v3Norm, float &dBRDF, RGB &rgbColor)
{
	float dN1, dN2, dCosT;

	dCosT = dot(v3In, v3Norm);
	if(dCosT < 0.0f) { // Incoming ray
		dN1 = 1.0f;
		dN2	= m_dIOR;
		dCosT = -dCosT;
	}
	else { // Outgoing ray
		dN1 = m_dIOR;
		dN2	= 1.0f;
	}

	float dN = dN1/dN2;
	float R0 = ((dN - 1) * (dN - 1)) / ((dN + 1) * (dN + 1));
	float dC = 1 - dCosT;

	float Re = R0 + (1 - R0) * dC * dC * dC * dC * dC;
	float Pr = (Re + 0.5) / 2.0;

	dBRDF = Re / Pr;
	rgbColor = this->radiance();

	if(dot(v3In, v3Norm) < 0.0f) { // Ray going in
		return reflect(v3In, v3Norm);
	} else { // Ray going out
		return reflect(v3In, -v3Norm);
	}
}

Vec3 DielectricMaterial::refractDir(Vec3 const &v3In, Vec3 const &v3Norm, float &dBRDF, RGB &rgbColor)
{
	float dN1, dN2, dCosT;

	dCosT = dot(v3In, v3Norm);
	if(dCosT < 0.0f) { // Incoming ray
		dN1 = 1.0f;
		dN2	= m_dIOR;
		dCosT = -dCosT;
	}
	else { // Outgoing ray
		dN1 = m_dIOR;
		dN2	= 1.0f;
	}

	float dN = dN1/dN2;
	float R0 = ((dN - 1) * (dN - 1)) / ((dN + 1) * (dN + 1));
	float dC = 1 - dCosT;

	float Re = R0 + (1 - R0) * dC * dC * dC * dC * dC;
	float Pr = (Re + 0.5) / 2.0;

	//dBRDF = (1.0 - Re) / (1.0 - Pr);
	dBRDF = 1 - (Re / Pr);
	rgbColor = this->radiance();

	if(dot(v3In, v3Norm) < 0.0f) { // Ray going in
		return refract(v3In, v3Norm, 1.0, m_dIOR);
	} else { // Ray going out
		return refract(v3In, -v3Norm, m_dIOR, 1.0);
	}
}

float DielectricMaterial::getReflectance(Vec3 const &v3In, Vec3 const &v3Norm)
{
	return 0.f;
}
