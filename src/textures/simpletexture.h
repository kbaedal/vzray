#ifndef __SIMPLETEXTURE_H__
#define __SIMPLETEXTURE_H__ 1

#include "textures/texture.h"

class SimpleTexture : public Texture
{
	public:
		SimpleTexture(RGB a_rgbColor) { rgbColor = a_rgbColor; }
		virtual RGB valor(Vec2 &a_v2Point, Vec3 &a_v3Point) const
		{
			return rgbColor;
		}
	
	RGB rgbColor;
};

#endif // __TEXTURE_H__
