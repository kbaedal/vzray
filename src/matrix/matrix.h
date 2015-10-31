#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include <iostream>
#include <iomanip>

#include "vec3.h"
#include "point.h"

/**
 * \class Definicion de una matriz cuadrada de 4x4 elementos.
 */
class Matrix4x4 {
    public:

        Matrix4x4();
        Matrix4x4(float m[4][4])
        {
			set(m);
		}
		Matrix4x4(float m[16])
        {
			set(m);
		}

        void set(float m[4][4]);
        void set(float m[16]);

		/**
		 * \brief Transforma la matriz en su inversa.
		 */
		void invert()
		{
			*this = get_inv();
		}

		/**
		 * \brief Devuelve la inversa de la matriz, sin modificarla.
		 */
        Matrix4x4 get_inv();

        /**
		 * \brief Trasforma la matriz en su traspuesta.
		 */
        void transpose()
        {
			*this = get_trans();
		}

        /**
		 * \brief Devuelve la traspuesta de la matriz, sin modificarla.
		 */
        Matrix4x4 get_trans();

        /**
         * \brief Calcula el cofactor del elemento e[X][Y].
         *
         * Devuelve el valor correspondiente, incluyendo el signo que le
         * corresponde a esa combinacion fila-columna.
         */
		float cofactor(int x, int y);

        /**
         * \brief Devuelve la matriz de cofactores, pero no la modifica.
         */
        Matrix4x4 get_cofactor();

        /**
         *  \brief Devuelve el determinante de la matriz.
         */
        float determinant();

        /**
         * \brief Aplicamos la matriz a un vector, a los que no les afectan traslaciones.
         */
        Vec3 transform(const Vec3 &v);

        /**
         * \brief Aplicamos la matriz a un punto, a los que sí les afectan traslaciones.
         */
        Point transform(const Point &p);

        friend Matrix4x4 operator+(const Matrix4x4 &m1, const Matrix4x4 &m2);
        friend Matrix4x4 operator-(const Matrix4x4 &m1, const Matrix4x4 &m2);
		friend Matrix4x4 operator*(const Matrix4x4 &m1, const Matrix4x4 &m2);
		friend Matrix4x4 operator/(const Matrix4x4 &m1, const Matrix4x4 &m2);

        friend Matrix4x4 operator*(const Matrix4x4 &m, float v);

        /**
         * \brief El operador / devolvera la matriz identidad si se intenta dividir por 0.
         */
        friend Matrix4x4 operator/(const Matrix4x4 &m, float v);

        friend inline std::ostream &operator<<(std::ostream &os, const Matrix4x4 &m)
        {
            for(int i = 0; i < 4; i++) {
                for(int j = 0; j < 4; j++) {
                    os << std::setw(11) << std::setprecision(5) << ((fabs(m.e[i][j]) < .00001) ? 0.f : m.e[i][j]) << " ";
                }
                os << std::endl;
            }

            return os;
        }

        float e[4][4];

    private:
        // Calcula el determinante de una matriz de 3x3. Pasamos los elementos
        // individualmente porque así podremos utilizarla para calcular el
        // determinante de una matriz de 4x4 elementos.
        float det3x3(
            float a11, float a12, float a13,
            float a21, float a22, float a23,
            float a31, float a32, float a33
        );
};

#endif // __MATRIX_HPP__
