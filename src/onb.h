#ifndef __ONB_H__
#define __ONB_H__ 1

#include <iostream>
#include <math.h>

#include "vec3.h"

/**
 * Clase ONB - Base Orto Normal
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
		 * @brief Incia la base ortonormal con el vector indicado
		 * @param a_u Vector U de la base
		 */		 
		void initFromU(const Vec3 &a_u);
		/**
		 * @brief Incia la base ortonormal con el vector indicado
		 * @param a_V Vector V de la base
		 */		 
		void initFromV(const Vec3 &a_v);
		/**
		 * @brief Incia la base ortonormal con el vector indicado
		 * @param a_W Vector W de la base
		 */		 
		void initFromW(const Vec3 &a_w);
		
		void set(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3) 
		{ 
			U = v1; 
			V = v2; 
			W = v3; 
		}

		/**
		 * Inicia la base con el par de vectores dados.
		 * 
		 * Estos vectores deben ser perpendiculares entre si, y estar
		 * normalizdos (su modulo debe ser 1).
		 */
		void initFromUV(const Vec3 &a_u, const Vec3 &a_v);
		void initFromVU(const Vec3 &a_v, const Vec3 &a_u);
		
		void initFromUW(const Vec3 &a_u, const Vec3 &a_w);
		void initFromWU(const Vec3 &a_w, const Vec3 &a_u);
		
		void initFromVW(const Vec3 &a_v, const Vec3 &a_w);
		void initFromWV(const Vec3 &a_w, const Vec3 &a_v);
		
		void normalizeVectors();
				
		Vec3 u() const { return U; }
		Vec3 v() const { return V; }
		Vec3 w() const { return W; }
		
		friend bool operator==(const ONB &o1, const ONB &o2);
		friend bool operator!=(const ONB &o1, const ONB &o2);
		
		friend std::ostream &operator<<(std::ostream &os, const ONB &onb);
			
		Vec3 U, V, W;
};

#endif
