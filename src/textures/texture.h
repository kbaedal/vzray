#ifndef __TEXTURE_H__
#define __TEXTURE_H__ 1

#include "vec3.h"
#include "vec2.h"
#include "rgb.h"

class Texture
{
	public:
	    virtual ~Texture() {};

		virtual RGB valor(Vec2 &a_v2Point, Vec3 &a_v3Point) const = 0;
};

#endif // __TEXTURE_H__
