#ifndef __TRANSFORM_HPP__
#define __TRANSFORM_HPP__

#include <iostream>

#include "matrix.h"

#include "vec3.h"
#include "point.h"
#include "ray.h"
#include "aabb/aabb.h"

/**
 * Clase que nos permitirá manejar las transformaciones aplicadas
 * a un objeto.
 */
class Transform {
    public:
        
        Transform()
        {
			m_mtxI = m_mtxM.getInv();
		}
        ~Transform() {};
        
        /**
         * Actualiza la transformación con los datos de otra transformacion.
         */
        void update(const Transform &a_tTrans);
        
        /**
         * Actualiza la transformacion con los datos de una matriz.
         */
        void update(const Matrix4x4 &a_mtxM);

        void translate(float dX, float dY, float dZ);
        void translate(const Point &p3Pos)
        {
			this->translate(p3Pos.x(), p3Pos.y(), p3Pos.z());
			m_mtxI = m_mtxM.getInv();
		}
		
        void scale(float dX, float dY, float dZ);
        void scale(const Point &p3Pos)
        {
			this->scale(p3Pos.x(), p3Pos.y(), p3Pos.z());
			m_mtxI = m_mtxM.getInv();
		}
		
        void rotate(float dAng, const Vec3 &a_v3Axis);
        void rotateX(float dAng);
        void rotateY(float dAng);
        void rotateZ(float dAng);

        void freeTrans(const Matrix4x4 &mtxF);
       
		/**
		 * Transformamos el rayo recibido en el espacio de la escena
		 * al espacio del objeto.
		 */
		Ray sceneToObject(const Ray &rayIn);
		
		/**
		 * Para el caso de que necesitemos transformar un punto del
		 * espacio de la escena al espacio del objeto.
		 */
		Point sceneToObject(const Point &p3Point);
		
		/**
		 * Transformamos el rayo recibido en el espacio del objeto
		 * al espacio de la escena.
		 */
		Ray objectToScene(const Ray &rayIn);
		
		/**
		 * Para el caso de que necesitemos transformar un punto del
		 * espacio del objeto al espacio de la escena.
		 */
		Point objectToScene(const Point &p3Point);
				
		/**
		 * La normal en el punto de intersección debe ser transformada
		 * de una forma particular, ya que es un caso especial.
		 */
		Vec3 objectNormalToScene(const Vec3 &v3Normal);
		
		/**
		 * Transformaremos la bbox indicada con los datos de la
		 * transformacion, ya que las bbox se almacenan siempre en
		 * espacio real.
		 */
		AABB updateAABB(const AABB &abBox);
		
		friend std::ostream& operator<<(std::ostream &os, const Transform &t);
		
		Matrix4x4 m_mtxM, m_mtxI;
};

#endif // __TRANSFORM_HPP__
