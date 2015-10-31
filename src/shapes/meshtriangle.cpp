#include <cstdio>

#include "isecaux.h"
#include "mesh.h"
#include "meshtriangle.h"

MeshTriangle::MeshTriangle(const MeshFace &face_info, Mesh *a_mesh)
{
	for(int i = 0; i < 3; i++) {
		vertices[i] = face_info.vertex_index[i];
		normals[i]  = face_info.normal_index[i];
		textures[i] = face_info.texture_index[i];
	}

	mesh = a_mesh;
}

bool MeshTriangle::hit(const Ray &r, float min_dist, float max_dist, HitRecord &hit) const
{
	float dist;

	Point 	p0, p1, p2;
	Vec3	v10, v20;

	p0 = mesh->vertex_list[vertices[0]];
	p1 = mesh->vertex_list[vertices[1]];
	p2 = mesh->vertex_list[vertices[2]];

	if(isecaux::test_ray_triangle(r, p0, p1, p2, min_dist, max_dist, dist))	{
		hit.dist = dist;

		if((normals[0] > -1) && (normals[1] > -1) && (normals[2] > -1)) {
			Vec3 n0, n1, n2;

			n0 = mesh->normal_list[normals[0]];
			n1 = mesh->normal_list[normals[1]];
			n2 = mesh->normal_list[normals[2]];

			hit.normal = versor(n0 + n1 + n2);
		}
		else {
			// No normal for each vertex, interpolate points
			v10.set(p1 - p0);
			v20.set(p2 - p0);

			hit.normal = versor(cross(v10, v20));
		}

		if((textures[0] > -1) && (textures[1] > -1) && (textures[2] > -1)) {
			// TODO
		}
		else {
			// No texture given, use mat.
			hit.material = mesh->material;
		}

		return true;
	}

	return false;
}

bool MeshTriangle::shadow_hit(const Ray &r, float min_dist, float max_dist) const
{
	float dist;

	Point p0, p1, p2;

	p0 = mesh->vertex_list[vertices[0]];
	p1 = mesh->vertex_list[vertices[1]];
	p2 = mesh->vertex_list[vertices[2]];

	return isecaux::test_ray_triangle(r, p0, p1, p2, min_dist, max_dist, dist);
}
