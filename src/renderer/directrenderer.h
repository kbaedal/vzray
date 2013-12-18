#ifndef __DIRECTRENDERER_H__
#define __DIRECTRENDERER_H__ 1

#include <vector>

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
		RGB getColor(Ray a_rRay, Scene *a_Scene, float a_dMin, float a_dMax, int a_nDepth);
		DirectRenderer(int a_nMaxDepth = 5, int a_nShadowSamps = 1, int a_nSeed = 65535)
		{
			m_nMaxDepth 	= a_nMaxDepth;
			m_nShadowSamps 	= a_nShadowSamps;
			m_MyRNG.RandomInit(a_nSeed);
		}
		
		int rendererType() { return 0; }
	
	private:
		RGB directLight(Point p3Point, Scene *a_Scene, HitRecord &htHit);
		
		int 			m_nMaxDepth;
		int				m_nShadowSamps;
		CRandomMersenne	m_MyRNG;
};

#endif // __DIRECTRENDERER_H__
