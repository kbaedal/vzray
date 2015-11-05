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
		Vec3(double a, double b, double c)	{ this->set(a, b, c); }
		Vec3() { e[0] = e[1] = e[2] = 0.0f; }
		Vec3(double a) { this->set(a); }
		Vec3(const Vec3 &v) { this->set(v); }
		Vec3(const Point &p);

		// Operadores aritmeticos
		const Vec3 &operator+() const { return *this; }
		Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
		friend Vec3 operator+(const Vec3 &v1, const Vec3 &v2);
		friend Vec3 operator-(const Vec3 &v1, const Vec3 &v2);
		friend Vec3 operator*(const Vec3 &v, double f);
		friend Vec3 operator*(double f, const Vec3 &v);
		friend Vec3 operator/(const Vec3 &v, double f);
		friend Vec3 operator/(double f, const Vec3 &v);

		// Operadores logicos
		friend bool operator==(const Vec3 &v1, const Vec3 &v2);
		friend bool operator!=(const Vec3 &v1, const Vec3 &v2);

		// Otras operaciones
		double length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
		double sq_length() const { return (e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
		void normalize() { *this = *this * (1.0f / length()); }
		void set(double a, double b, double c)
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
		void set(double a)
		{
			e[0] = e[1] = e[2] = a;
		}
		void set(double *a)
		{
		    e[0] = a[0];
		    e[1] = a[1];
		    e[2] = a[2];
		}
		void set(const Point &p);

		/**
		 * Producto escalar de vectores
		 */
		friend double dot(Vec3 const &v1, Vec3 const &v2);

		/**
		 * Producto vectorial
		 */
		friend Vec3 cross(Vec3 const &v1, Vec3 const &v2);

		/**
		 * Producto triple de vectores.
		 *
		 * Se define como dot(cross(v1, v2), v3)
		 */
		friend double triple_product(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3);

		/**
		 * \brief Devuelve el versor (vector modulo = 1) asociado al parametro indicado.
		 *
		 * \param v Vector del que calcular su versor
		 * \return El versor correspondiente
		 */
		friend Vec3 versor(const Vec3 &v);

		/**
		 * \brief Calcula el vector reflejado correspondiente al vector incidente.
		 *
		 * \param v3In Vector incidente sobre la superficie.
		 * \param v3Normal Vector normal en el punto de interseccion.
		 * \return La direccion de la reflexión.
		 */
		friend Vec3 reflect(
			const Vec3 &in, 	// Incoming vector
			const Vec3 &norm	// Normal at intersection point
		);

		/**
		 * \brief Calcula el vector refractado correspondiente a las
		 * características de las superficies y el vector incidente.
		 *
		 * \param v3In Vector incidente sobre la superficie.
		 * \param v3Normal Vector normal en el punto de interseccion.
		 * \param dExtIOR Indice de refraccion del medio externo.
		 * \param dIntIOR Indice de refracción del medio interno.
		 * \return La direccion de la refracción.
		 */
		friend Vec3 refract(
			const Vec3 &in, 		// Incoming vector
			const Vec3 &norm, 	// Normal at intersection point
			const double ext_ior, 	// External index of refraction
			const double int_ior		// Internal index of refraction
		);

		/**
		 * \brief Calcula un vector normalizado en una direccion aleatoria
		 * que estará dentro del hemisferio definido por v3Dir.
		 *
		 * \param dir Vector que define el hemisferio.
		 * \param rn1 Numero aleatorio tal que 0.f <= rn1 < 1.f
		 * \param rn2 Numero aleatorio tal que 0.f <= rn2 < 1.f
		 * \return Vector de direccion aleatoria en el hemisferio.
		 */
		friend Vec3 random_dir(const Vec3 &dir, double rn1, double rn2);

		// Acceso rapido a los componentes
		double x() const { return e[0]; }
		double y() const { return e[1]; }
		double z() const { return e[2]; }

		// Manejo del operador << para ostream
		friend std::ostream& operator<<(std::ostream &os, const Vec3 &v);

		// Componentes
		double e[3];
};

#endif // __VEC3_H__
