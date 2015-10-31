#include "material/lightmaterial.h"
#include "vec2.h"
#include "vec3.h"

RGB LightMaterial::radiance()
{
	Vec2 v2;
	Vec3 v3;
	return texture->valor(v2, v3);
}

RGB LightMaterial::emittance()
{
	Vec2 v2;
	Vec3 v3;
	return texture->valor(v2, v3);
}
