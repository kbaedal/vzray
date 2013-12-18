#include "material/diffusematerial.h"
#include "vec2.h"
#include "vec3.h"

RGB DiffuseMaterial::radiance()
{
	Vec2 unVec2;
	Vec3 unVec3;
	return m_pTexture->valor(unVec2, unVec3);
}

Vec3 DiffuseMaterial::outDirection(Vec3 const &v3In, Vec3 const &v3Norm, float &dBRDF, RGB &rgbColor, CRandomMersenne *rng)
{
	// Generamos una nueva direccion. Sera una direccion aleatoria en la
	// semiesfera definida por la normal.
	dBRDF = 1.0;
	rgbColor = this->radiance();
	return versor(randomDir(v3Norm, rng->Random(), rng->Random()));
}
