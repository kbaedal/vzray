#ifndef __RENDERER_H__
#define __RENDERER_H__ 1

#include <vector>

#include "ray.h"
#include "vec3.h"
#include "rgb.h"
#include "shapes/shape.h"
#include "scene/scene.h"

using namespace std;

class Renderer
{
	public:
	    virtual ~Renderer() {};
		virtual RGB getColor(Ray a_rRay, Scene *a_Scene, float a_dMin, float a_dMax, int a_nDepth) = 0;

		virtual int rendererType() = 0;
};

#endif // __RENDERER_H__
