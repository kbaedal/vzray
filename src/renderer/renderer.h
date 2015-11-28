#ifndef __RENDERER_H__
#define __RENDERER_H__ 1

#include <vector>
#include <string>

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
		virtual RGB get_color(Ray r, Scene *scene, double min_dist, double max_dist, int depth) = 0;
		virtual Contrib get_color_v2(Ray r, Scene *scene, double min_dist, double max_dist, int depth) = 0;


		virtual std::string renderer_type() = 0;
};

#endif // __RENDERER_H__
