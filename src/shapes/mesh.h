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
		MeshFace(int n) {
			set(n);
		}
		MeshFace() { set(0); }

		void set(int n) { // Iniciamos los datos todos al mismo valor
			for(int i = 0; i < 3; i++) {
				vertex_index[i] = n;
				normal_index[i] = n;
				texture_index[i] = n;
			}
		}

		int vertex_index[3]; // Vertex index in m_v3VertexArray
		int normal_index[3];	// Normal index in m_v3NormalArray
		int texture_index[3];	// Texture index in m_v2TextureArray
};

class Mesh : public Shape
{
	public:
		Mesh()
		{
			num_triangles	= 0;
			trans           = new Transform;
		}

		~Mesh()	{ if(trans != NULL) delete trans; }


		// A Mesh cannot be intersected
		bool hit(const Ray &r, double min_dist, double max_dist, HitRecord &hit) const
		{
			return false;
		}
		bool shadow_hit(const Ray &r, double min_dist, double max_dist) const
		{
			return false;
		}
		bool get_random_point(const Point &view_pos, CRandomMersenne *rng, Point &light_pos) const
		{
			return false;
		}

		// Add info to internal arrays
		void add_vertex(const Point &p);
		void add_normal(const Vec3 &n);
		void add_texture(const Vec2 &t);

		// Add triangle info (vertex index, normal index and texture index)
		// void addTriangle(const MeshFace &mfFace);

		int get_triangle_count() { return num_triangles; }
		void increase_triangle_count() { ++num_triangles; }

		friend class MeshTriangle;

	private:
		int						num_triangles;

		std::vector<Point> 		vertex_list;
		std::vector<Vec3>		normal_list;
		std::vector<Vec2>		texture_list;
};

#endif // __MESH_HPP__
