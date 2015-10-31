#ifndef __MESHTRIANGLE_HPP__
#define __MESHTRIANGLE_HPP__ 1

#include "shape.h"
#include "mesh.h"

#include "vec3.h"
#include "ray.h"
#include "rgb.h"

class MeshTriangle : public Shape
{
	public:
		MeshTriangle(const MeshFace &face_info, Mesh *a_mesh);
		// Y EL DESTRUCTOR?
		// QUIEN SE ENCARGA DE BORRAR pMEsh?? la escena??

		bool hit(const Ray &r, float min_dist, float max_dist, HitRecord &hit) const;
		bool shadow_hit(const Ray &a, float min_dist, float max_dist) const;
		bool get_random_point(const Point &view_pos, CRandomMersenne *rng, Point &light_pos) const
		{
			light_pos = Point(0.f);
			return true;
		}

	private:
		// Lista de índices (vertices, normales, texturas)
		int		vertices[3];
		int		normals[3];
		int		textures[3];

		// Puntero al objeto mesh al que pertenece el triángulo
		Mesh	*mesh;
};

#endif // __MESHTRIANGLE_HPP__
