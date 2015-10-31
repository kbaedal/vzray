#ifndef __SIMPLETEXTURE_H__
#define __SIMPLETEXTURE_H__ 1

#include "textures/texture.h"

class SimpleTexture : public Texture
{
	public:
		SimpleTexture(RGB a_color) { color = a_color; }
		virtual RGB valor(Vec2 &a_p2d, Vec3 &a_p3d) const
		{
			return color;
		}

	RGB color;
};

#endif // __TEXTURE_H__
