#ifndef __DIRECTRENDERER_H__
#define __DIRECTRENDERER_H__ 1

#include <vector>
#include <string>

#include "renderer.h"

#include "ray.h"
#include "vec3.h"
#include "point.h"
#include "rgb.h"
#include "shapes/shape.h"
#include "randomc/randomc.h"
#include "scene/scene.h"

class DirectRenderer : public Renderer
{
	public:
		DirectRenderer(int a_max_depth = 5, int a_shadow_samps = 1, int seed = 65535) :
            max_depth(a_max_depth), shadow_samps(a_shadow_samps)
		{
			rng.RandomInit(seed);
		}

		RGB get_color(Ray r, Scene *scene, double min_dist, double max_dist, int depth) final;
		Contrib get_color_v2(Ray r, Scene *scene, double min_dist, double max_dist, int depth) final;

		std::string renderer_type() { return std::string("Path + Direct raytracing"); }

	private:
		RGB direct_light(Point p, Scene *scene, HitRecord &hit);

		int 			max_depth;
		int				shadow_samps;
		CRandomMersenne	rng;

		static const double kepsilon; // Autohit.
};

#endif // __DIRECTRENDERER_H__
