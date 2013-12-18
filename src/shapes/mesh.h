#ifndef __MESH_HPP__
#define __MESH_HPP__ 1

#include <vector>

#include "shape.h"

#include "vec3.h"
#include "point.h"
#include "vec2.h"
#include "ray.h"
#include "rgb.h"

class MeshFace {
	public:
		MeshFace(int nValor) { 
			this->set(nValor);
		}
		MeshFace() { this->set(0); }
		
		void set(int nValor) { // Iniciamos los datos todos al mismo valor
			for(int i = 0; i < 3; i++) {
				nVIndex[i] = nValor;
				nNIndex[i] = nValor;
				nTIndex[i] = nValor;
			}
		}
		
		int nVIndex[3]; // Vertex index in m_v3VertexArray 
		int nNIndex[3];	// Normal index in m_v3NormalArray
		int nTIndex[3];	// Texture index in m_v2TextureArray
};

class Mesh : public Shape
{
	public:
		Mesh()
		{
			m_nNumT	= 0;
			m_pTrans = new Transform;
		}
	
		~Mesh()	{ if(m_pTrans != NULL) delete m_pTrans; }
	
		
		// A Mesh cannot be intersected
		bool hit(const Ray &rRay, float dMin, float dMax, HitRecord &hrHitRcd) const
		{
			return false;
		}
		bool shadowHit(const Ray &rRay, float dMin, float dMax) const
		{
			return false;
		}
		bool getRandomPoint(const Point &p3ViewPoint, CRandomMersenne *rngGen, Point &p3LPoint) const
		{
			return false;
		}		
		
		// Add info to internal arrays
		void addVertex(const Point &p);
		void addNormal(const Vec3 &n);
		void addTexture(const Vec2 &t);
		
		// Add triangle info (vertex index, normal index and texture index)
		// void addTriangle(const MeshFace &mfFace);
		
		int getTriangleCount() { return m_nNumT; }
		void increaseTriangleCount() { m_nNumT++; }
		
		friend class MeshTriangle;
		
	private:
		int						m_nNumT;
		
		std::vector<Point> 		m_p3VertexArray;
		std::vector<Vec3>		m_v3NormalArray;
		std::vector<Vec2>		m_v2TextureArray;
};

#endif // __MESH_HPP__
