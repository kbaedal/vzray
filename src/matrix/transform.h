#ifndef __TRANSFORM_HPP__
#define __TRANSFORM_HPP__

#include <iostream>

#include "matrix.h"

#include "vec3.h"
#include "point.h"
#include "ray.h"
#include "aabb/aabb.h"

/**
 * \class Clase que nos permitirá manejar las transformaciones aplicadas
 * a un objeto.
 */
class Transform {
    public:

        Transform()
        {
			inv = mtx.get_inv();
		}
        ~Transform() {};

        /**
         * \brief Actualiza la transformación con los datos de otra transformacion.
         */
        void update(const Transform &t);

        /**
         * \brief Actualiza la transformacion con los datos de una matriz.
         */
        void update(const Matrix4x4 &m);

        void translate(float x, float y, float z);
        void translate(const Point &p)
        {
			translate(p.x(), p.y(), p.z());
			inv = mtx.get_inv();
		}

        void scale(float x, float y, float z);
        void scale(const Point &p)
        {
			scale(p.x(), p.y(), p.z());
			inv = mtx.get_inv();
		}

        void rotate(float angle, const Vec3 &axis);
        void rotate_x(float angle);
        void rotate_y(float angle);
        void rotate_z(float angle);

        void free_transform(const Matrix4x4 &m);

		/**
		 * \brief Transformamos el rayo recibido en el espacio de la escena
		 * al espacio del objeto.
		 */
		Ray scene_to_object(const Ray &r);

		/**
		 * \brief Para el caso de que necesitemos transformar un punto del
		 * espacio de la escena al espacio del objeto.
		 */
		Point scene_to_object(const Point &p);

		/**
		 * \brief Transformamos el rayo recibido en el espacio del objeto
		 * al espacio de la escena.
		 */
		Ray object_to_scene(const Ray &r);

		/**
		 * \brief Para el caso de que necesitemos transformar un punto del
		 * espacio del objeto al espacio de la escena.
		 */
		Point object_to_scene(const Point &p);

		/**
		 * \brief La normal en el punto de intersección debe ser transformada
		 * de una forma particular, ya que es un caso especial.
		 */
		Vec3 normal_to_scene(const Vec3 &n);

		/**
		 * \brief Transformaremos la bbox indicada con los datos de la
		 * transformacion, ya que las bbox se almacenan siempre en
		 * espacio real.
		 */
		AABB update_AABB(const AABB &b);

		friend std::ostream& operator<<(std::ostream &os, const Transform &t);

		Matrix4x4 mtx, inv;

    private:
        // Constantes utiles
        static const float kpi;
        static const float kmin_value;

};

#endif // __TRANSFORM_HPP__
