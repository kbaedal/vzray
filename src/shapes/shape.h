#ifndef __SHAPE_H__
#define __SHAPE_H__ 1

#include "vec3.h"
#include "point.h"
#include "ray.h"
#include "rgb.h"
#include "material/material.h"
#include "matrix/transform.h"
#include "randomc/randomc.h"
#include "aabb/aabb.h"

class Shape
{
	public:
		virtual ~Shape() {};

		virtual bool hit(const Ray &a_rRay, float a_dMin, float a_dMax, HitRecord &a_hrHitRcd) const = 0;
		virtual bool shadowHit(const Ray &a_rRay, float a_dMin, float a_dMax) const = 0;
		virtual bool getRandomPoint(const Point &p3ViewPoint, CRandomMersenne *rngGen, Point &p3LPoint) const = 0;

		Material *getMaterial() { return m_pMat; }
		void setMaterial(Material *a_pMat) { m_pMat = a_pMat; }
		Transform *getTransform() { return m_pTrans; }
		void setTrans(const Transform *a_pTrans)
		{
			// Update transform
			m_pTrans->update(*a_pTrans);

			// Update AABB
			Transform tTemp;

			tTemp.update(*a_pTrans);
			m_abBox = tTemp.updateAABB(m_abBox);
		}

		AABB getAABB() { return m_abBox; }

		void setShadow(bool a_bShadow) { bShadow = a_bShadow; }
		bool castShadow() { return bShadow; }

		void setBounds(bool a_bBounds) { bBounds = a_bBounds; }
		bool applyBounds() { return bBounds; }

	//~ protected:
		Material	*m_pMat;
		Transform	*m_pTrans;

		AABB		m_abBox;

		// Proyectar sombra?
		bool		bShadow;
		// Aplicar abBox?
		bool		bBounds;
};

#endif // __SHAPE_HPP__
