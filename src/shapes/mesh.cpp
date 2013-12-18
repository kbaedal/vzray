#include "mesh.h"

#include <cmath>

void Mesh::addVertex(const Point &p)
{
	m_p3VertexArray.push_back(p);
}

void Mesh::addNormal(const Vec3 &n)
{
	m_v3NormalArray.push_back(n);
}

void Mesh::addTexture(const Vec2 &t)
{
	m_v2TextureArray.push_back(t);
}
