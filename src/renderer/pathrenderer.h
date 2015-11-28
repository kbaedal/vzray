#ifndef __PATHRENDERER_H__
#define __PATHRENDERER_H__ 1

#include <vector>
#include <string>

#include "renderer/renderer.h"
#include "ray.h"
#include "vec3.h"
#include "rgb.h"
#include "shapes/shape.h"
#include "randomc/randomc.h"
#include "scene/scene.h"

class PathRenderer : public Renderer
{
	public:
	    PathRenderer(int a_max_depth = 5, int seed = 65535) : max_depth(a_max_depth)
		{
			rng.RandomInit(seed);
		}

		RGB get_color(Ray r, Scene *scene, double min_dist, double max_dist, int depth) final;
		Contrib get_color_v2(Ray r, Scene *scene, double min_dist, double max_dist, int depth) final;


		std::string renderer_type() { return std::string("Pathtracing"); }

	private:
		int 			max_depth;
		CRandomMersenne	rng;

		static const double kepsilon; // Autohit.
};

#endif // __PATHRENDERER_H__
