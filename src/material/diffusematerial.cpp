#include "material/diffusematerial.h"
#include "vec2.h"
#include "vec3.h"

RGB DiffuseMaterial::radiance()
{
	Vec2 v2;
	Vec3 v3;
	return texture->valor(v2, v3);
}

Vec3 DiffuseMaterial::out_direction(Vec3 const &in, Vec3 const &norm, float &brdf, RGB &color, CRandomMersenne *rng)
{
	// Generamos una nueva direccion. Sera una direccion aleatoria en la
	// semiesfera definida por la normal.
	brdf = 1.0;
	color = radiance();
	return versor(random_dir(norm, rng->Random(), rng->Random()));
}
