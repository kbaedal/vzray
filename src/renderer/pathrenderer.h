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
		RGB getColor(Ray a_rRay, Scene *a_Scene, float a_dMin, float a_dMax, int a_nDepth);
		PathRenderer(int a_nMaxDepth = 5, int a_nSeed = 65535)
		{
			m_nMaxDepth 	= a_nMaxDepth;
			m_MyRNG.RandomInit(a_nSeed);
		}
		
		int rendererType() { return 1; }
	
	private:
	
		int 			m_nMaxDepth;
		CRandomMersenne	m_MyRNG;
};

#endif // __PATHRENDERER_H__
