#ifndef __ONB_H__
#define __ONB_H__ 1

#include <iostream>
#include <math.h>

#include "vec3.h"

/**
 * \class Base Orto Normal
 *
 * Implementa las operaciones necesarias para manejar una
 * base ortonormal de vectores (tres vectores perpendiculares
 * entre si)
 */
class ONB
{
	public:
		ONB() {};
		ONB(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3)
		{
			this->set(v1, v2, v3);
		}

		/**
		 * \brief Incia la base ortonormal con el vector indicado
		 *
		 * \param a_u Vector u de la base
		 */
		void init_from_u(const Vec3 &a_u);
		/**
		 * \brief Incia la base ortonormal con el vector indicado
		 *
		 * \param a_v Vector v de la base
		 */
		void init_from_v(const Vec3 &a_v);
		/**
		 * \brief Incia la base ortonormal con el vector indicado
		 *
		 * \param a_w Vector w de la base
		 */
		void init_from_w(const Vec3 &a_w);

		void set(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3)
		{
			U = v1;
			V = v2;
			W = v3;
		}

		/**
		 * \brief Inicia la base con el par de vectores dados.
		 *
		 * Estos vectores deben ser perpendiculares entre si, y estar
		 * normalizdos (su modulo debe ser 1).
		 */
		void init_from_uv(const Vec3 &a_u, const Vec3 &a_v);
		void init_from_vu(const Vec3 &a_v, const Vec3 &a_u);

		void init_from_uw(const Vec3 &a_u, const Vec3 &a_w);
		void init_from_wu(const Vec3 &a_w, const Vec3 &a_u);

		void init_from_vw(const Vec3 &a_v, const Vec3 &a_w);
		void init_from_wv(const Vec3 &a_w, const Vec3 &a_v);

		void normalize_vectors();

		Vec3 u() const { return U; }
		Vec3 v() const { return V; }
		Vec3 w() const { return W; }

		friend bool operator==(const ONB &o1, const ONB &o2);
		friend bool operator!=(const ONB &o1, const ONB &o2);

		friend std::ostream &operator<<(std::ostream &os, const ONB &onb);

		Vec3 U, V, W;

    private:
        static const float konb_epsilon;
};

#endif
