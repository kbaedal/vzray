#ifndef __PATHRENDERER_H__
#define __PATHRENDERER_H__ 1

#include <vector>

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
		RGB get_color(Ray r, Scene *scene, float min_dist, float max_dist, int depth);
		PathRenderer(int a_max_depth = 5, int seed = 65535)
		{
			max_depth = a_max_depth;
			rng.RandomInit(seed);
		}

		int renderer_type() { return 1; }

	private:
		int 			max_depth;
		CRandomMersenne	rng;

		static const float kepsilon; // Autohit.
};

#endif // __PATHRENDERER_H__
