#include "mesh.h"

#include <cmath>

void Mesh::add_vertex(const Point &p)
{
	vertex_list.push_back(p);
}

void Mesh::add_normal(const Vec3 &n)
{
	normal_list.push_back(n);
}

void Mesh::add_texture(const Vec2 &t)
{
	texture_list.push_back(t);
}
