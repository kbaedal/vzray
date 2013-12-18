#include "material/lightmaterial.h"
#include "vec2.h"
#include "vec3.h"

RGB LightMaterial::radiance()
{
	Vec2 unVec2;
	Vec3 unVec3;
	return m_pTexture->valor(unVec2, unVec3);
}

RGB LightMaterial::emittance()
{
	Vec2 unVec2;
	Vec3 unVec3;
	return m_pTexture->valor(unVec2, unVec3);
}
