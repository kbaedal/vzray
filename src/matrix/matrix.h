#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include <iostream>
#include <iomanip>

#include "vec3.h"
#include "point.h"

/**
 * Definicion de una matriz cuadrada de 4x4 elementos.
 */
class Matrix4x4 {
    public:
    
        Matrix4x4();
        Matrix4x4(float dMatrix[4][4])
        {
			this->set(dMatrix);
		}
		Matrix4x4(float dMatrix[16])
        {
			this->set(dMatrix);
		}

        void set(float dMatrix[4][4]);
        void set(float dMatrix[16]);
		
		/**
		 * Transforma la matriz en su inversa.
		 */
		void invert()
		{
			*this = this->getInv();
		}
		
		/**
		 * Devuelve la inversa de la matriz, sin modificarla.
		 */
        Matrix4x4 getInv();
        
        /**
		 * Trasforma la matriz en su traspuesta.
		 */
        void transpose()
        {
			*this = this->getTrans();
		}
        
        /**
		 * Devuelve la traspuesta de la matriz, sin modificarla.
		 */
        Matrix4x4 getTrans();
        
        /**
         * Calcula el cofactor del elemento e[X][Y]. Devuelve el valor
         * correspondiente, incluyendo el signo que le corresponde a
         * esa combinacion fila-columna.
         */
		float cofactorXY(int X, int Y);
		
        /** 
         * Devuelve la matriz de cofactores, pero no la modifica.
         */
        Matrix4x4 getCofactor();
        
        /**
         *  Devuelve el determinante de la matriz.
         */
        float determinant();
        
		/**
		 * Tenemos que distinguir entre si transformamos un vec (no 
		 * afectados por traslaciones) o si transformamos un punto 
		 * (sí afectado). Así pues, forzaremos a hacer la transformación
		 * de forma explícita.
		 */
        Vec3 transform(const Vec3 &v3Vector);
        Point transform(const Point &p3Point);
        
        friend Matrix4x4 operator+(const Matrix4x4 &mtxM1, const Matrix4x4 &mtxM2);
        friend Matrix4x4 operator-(const Matrix4x4 &mtxM1, const Matrix4x4 &mtxM2);
		friend Matrix4x4 operator*(const Matrix4x4 &mtxM1, const Matrix4x4 &mtxM2);
		friend Matrix4x4 operator/(const Matrix4x4 &mtxM1, const Matrix4x4 &mtxM2);
		
        friend Matrix4x4 operator*(const Matrix4x4 &mtxMatrix, float dV);
        
        /**
         * El operador / devolvera la matriz identidad si se intenta dividir por 0.
         */
        friend Matrix4x4 operator/(const Matrix4x4 &mtxMatrix, float dV);
        
        float e[4][4];
};

/**
 * Calcula el determinante de una matriz de 3x3. Pasamos los elementos 
 * individualmente porque así podremos utilizarla para calcular el
 * determinante de una matriz de 4x4 elementos.
 */
float det3(
	float a11, float a12, float a13,
	float a21, float a22, float a23,
	float a31, float a32, float a33
);

inline std::ostream& operator<<(std::ostream &os, const Matrix4x4 &m)
{
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			os << std::setw(11) << std::setprecision(5) << ((fabs(m.e[i][j]) < .00001) ? 0.f : m.e[i][j]) << " ";
		}
		os << std::endl;
	}
	
	return os;
}

#endif // __MATRIX_HPP__
