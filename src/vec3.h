#ifndef __VEC3_H__
#define __VEC3_H__ 1

#include <cmath>
#include <iostream>

class Point;

/**
 * Manejo de vectores de 3 coordenadas.
 */
class Vec3
{
	public:
		// Constructores
		Vec3(float a, float b, float c)
		{
			this->set(a, b, c);
		}
		Vec3() { e[0] = e[1] = e[2] = 0.0f; }
		Vec3(float a) { this->set(a); }
		Vec3(const Vec3 &v) { this->set(v); }
		
		// Operadores aritmeticos
		const Vec3 &operator+() const { return *this; }
		Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
		friend Vec3 operator+(const Vec3 &v1, const Vec3 &v2);
		friend Vec3 operator-(const Vec3 &v1, const Vec3 &v2);
		friend Vec3 operator*(const Vec3 &v, float f);
		friend Vec3 operator*(float f, const Vec3 &v);
		friend Vec3 operator/(const Vec3 &v, float f);
		friend Vec3 operator/(float f, const Vec3 &v);
		
		// Operadores logicos
		friend bool operator==(const Vec3 &v1, const Vec3 &v2);
		friend bool operator!=(const Vec3 &v1, const Vec3 &v2);
		
		// Otras operaciones
		float length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
		float sqLength() const { return (e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
		void normalize() { *this = *this * (1 / this->length()); }
		void set(float a, float b, float c)
		{
			e[0] = a;
			e[1] = b;
			e[2] = c;	
		}
		void set(const Vec3 &v)
		{
			e[0] = v.x();
			e[1] = v.y();
			e[2] = v.z();	
		}
		void set(float a)
		{
			e[0] = e[1] = e[2] = a;
		}
		void set(const Point &p);
		
		/**
		 * Producto escalar de vectores
		 */
		friend float dot(Vec3 const &v1, Vec3 const &v2);
		
		/**
		 * Producto vectorial
		 */
		friend Vec3 cross(Vec3 const &v1, Vec3 const &v2);
		
		/**
		 * Producto triple de vectores.
		 * 
		 * Se define como dot(cross(v1, v2), v3)
		 */
		friend float tripleProduct(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3);

		/**
		 * Devuelve el versor asociado al parametro indicado.
		 * @param v Vector del que calcular su versor
		 * @return El versor correspondiente
		 */	
		friend Vec3 versor(const Vec3 &v);
		
		/**
		 * Calcula el vector reflejado correspondiente al vector incidente.
		 * @param v3In Vector incidente sobre la superficie.
		 * @param v3Normal Vector normal en el punto de interseccion.
		 * @return La direccion de la reflexión.
		 */
		friend Vec3 reflect(
			const Vec3 &v3In, 	// Incoming vector
			const Vec3 &v3Norm	// Normal at intersection point
		);
		
		/**
		 * Calcula el vector refractado correspondiente a las
		 * características de las superficies y el vector incidente.
		 * @param v3In Vector incidente sobre la superficie.
		 * @param v3Normal Vector normal en el punto de interseccion.
		 * @param dExtIOR Indice de refraccion del medio externo.
		 * @param dIntIOR Indice de refracción del medio interno.
		 * @return La direccion de la refracción.
		 */
		friend Vec3 refract(
			const Vec3 &v3In, 		// Incoming vector
			const Vec3 &v3Norm, 	// Normal at intersection point
			const float dExtIOR, 	// External index of refraction
			const float dIntIOR		// Internal index of refraction
		);
		
		
		/**
		 * Calcula un vector normalizado en una direccion aleatoria
		 * que estará dentro del hemisferio definido por v3Dir.
		 * @param v3Dir Vector que define el hemisferio.
		 * @param dRN1 Numero aleatorio tal que 0.f <= dRN1 < 1.f
		 * @param dRN2 Numero aleatorio tal que 0.f <= dRN2 < 1.f
		 * @return Vector de direccion aleatoria en el hemisferio.
		 */
		friend Vec3 randomDir(const Vec3 &v3Dir, float dRN1, float dRN2);
		
		// Acceso rapido a los componentes
		float x() const { return e[0]; }
		float y() const { return e[1]; }
		float z() const { return e[2]; }
					
		// Manejo del operador << para ostream
		friend std::ostream& operator<<(std::ostream &os, const Vec3 &v);
		
		// Componentes
		float e[3];
};

#endif // __VEC3_H__
