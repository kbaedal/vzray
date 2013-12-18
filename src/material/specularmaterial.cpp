#include "material/specularmaterial.h"
#include "vec2.h"
#include "vec3.h"

RGB SpecularMaterial::radiance()
{
	Vec2 unVec2;
	Vec3 unVec3;
	return m_pTexture->valor(unVec2, unVec3);
}
