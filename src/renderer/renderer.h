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
		virtual RGB get_color(Ray r, Scene *scene, float min_dist, float max_dist, int depth) = 0;

		virtual int renderer_type() = 0;
};

#endif // __RENDERER_H__
