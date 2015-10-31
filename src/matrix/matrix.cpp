#include <vector>

#include "matrix/matrix.h"

Matrix4x4::Matrix4x4()
{
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(i==j)
				e[i][j] = 1.0;
            else
				e[i][j] = 0.0;
		}
    }
}

void Matrix4x4::set(float m[4][4])
{
	for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
			e[i][j] = m[i][j];
}

void Matrix4x4::set(float m[16])
{
	for(int i = 0, k = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
			e[i][j] = m[k++];
}

Matrix4x4 Matrix4x4::get_inv()
{
	// Aplicaremos el método de cálculo por determinates:
	// Sea una matriz A, se define I(A) como su matriz inversa:
	//
	//                  T(C(A))
	//			I(A) = ----------
	//                    |A|
	//
	// Donde C(A) es la matriz de cofactores de A, T(A) es la transpuesta
	// de A y |A| es el determinante de A.
	// Luego A tiene inversa si y solo si su determinante es distinto de 0.

	Matrix4x4 mtemp;

	// Calculamos el determinante. Si es cero, devolvemos la matriz identidad.
	float det = determinant();

	if(det != 0.0f) {
		// Calculamos la matriz de cofactores
		mtemp = get_cofactor();

		// La transponemos
		mtemp.transpose();

		// Y la dividimos por el determinante.
		mtemp = mtemp / det;
	}
	else {
		std::clog << "----\nMatriz SIN inversa posible.\n---\n" << std::endl;
		std::clog << *this << std::endl;
		std::clog << "Determinante: " << det << std::endl;
	}

	return mtemp;
}

Matrix4x4 Matrix4x4::get_trans()
{
	Matrix4x4 mtemp;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            mtemp.e[i][j] = e[j][i];

    return mtemp;
}

Matrix4x4 Matrix4x4::get_cofactor()
{
	Matrix4x4 mtemp;

	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			mtemp.e[i][j] = cofactor(i, j);

    return mtemp;
}

Matrix4x4 operator+(const Matrix4x4 &m1, const Matrix4x4 &m2)
{
	Matrix4x4 mtemp;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            mtemp.e[i][j] = m1.e[i][j] + m2.e[i][j];

    return mtemp;
}

Matrix4x4 operator-(const Matrix4x4 &m1, const Matrix4x4 &m2)
{
	Matrix4x4 mtemp;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            mtemp.e[i][j] = m1.e[i][j] - m2.e[i][j];

    return mtemp;
}

Matrix4x4 operator*(const Matrix4x4 &m1, const Matrix4x4 &m2)
{
    Matrix4x4 mtemp;

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            float sum = 0.0;
            for(int k = 0; k < 4; k++)
				sum += m1.e[i][k] * m2.e[k][j];

            mtemp.e[i][j] = sum;
        }
    }

    return mtemp;
}

Matrix4x4 operator/(const Matrix4x4 &m1, const Matrix4x4 &m2)
{
    Matrix4x4 mres, mtemp;

    mtemp = m2;
    mtemp.invert();

    mres = m1 * mtemp;

    return mres;
}

Vec3 Matrix4x4::transform(const Vec3 &v)
{
	float   componentes[4],
            nuevas_comp[4],
            res;

    componentes[0] = v.x();
    componentes[1] = v.y();
    componentes[2] = v.z();
    componentes[3] = 0.0;

    for(int i = 0; i < 4; i++) {
        res = 0.0;

        for(int j = 0; j < 4; j++)
            res += e[i][j] * componentes[j];

        if((res > 0.0) || (res < 0.0))
            nuevas_comp[i] = res;
        else
            nuevas_comp[i] = 0.0f;
    }

    return Vec3(nuevas_comp[0], nuevas_comp[1], nuevas_comp[2]);
}

Point Matrix4x4::transform(const Point &p)
{
	float   componentes[4],
            nuevas_comp[4],
            res;

    componentes[0] = p.x();
    componentes[1] = p.y();
    componentes[2] = p.z();
    componentes[3] = 1.0;

    for(int i = 0; i < 4; i++) {
        res = 0.0;

        for(int j = 0; j < 4; j++) {
            res += e[i][j] * componentes[j];
        }

        if((res > 0.0) or (res < 0.0))
            nuevas_comp[i] = res;
        else
            nuevas_comp[i] = 0.0f;
    }

    return Point(nuevas_comp[0], nuevas_comp[1], nuevas_comp[2]);
}

Matrix4x4 operator*(const Matrix4x4 &m, float v)
{
	Matrix4x4 mtemp;

	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			mtemp.e[i][j] = m.e[i][j] * v;

	return mtemp;
}

Matrix4x4 operator/(const Matrix4x4 &m, float v)
{
	Matrix4x4 mtemp;

	if(v != 0.0) {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				mtemp.e[i][j] = m.e[i][j] / v;
	}

	return mtemp;
}

float Matrix4x4::determinant()
{
	// La funcion CofactorXY ya devuelve el signo adecuado para la
	// posición calculada, con lo que simplemente tenemos que sumar.
	return  e[0][0] * cofactor(0, 0) +
            e[0][1] * cofactor(0, 1) +
            e[0][2] * cofactor(0, 2) +
            e[0][3] * cofactor(0, 3);
}

float Matrix4x4::det3x3(
	float a11, float a12, float a13,
	float a21, float a22, float a23,
	float a31, float a32, float a33
)
{
	// Aplicamos la regla de Sarrus
	return  a11 * a22 * a33 +
            a12 * a23 * a31 +
            a13 * a21 * a32 -
            a13 * a22 * a31 -
            a12 * a21 * a33 -
            a11 * a23 * a32;
}

float Matrix4x4::cofactor(int x, int y)
{
	std::vector<float> m3;

	// Almacenamos por orden los 9 numeros resultantes de eliminar
	// de la matriz de 4x4 la fila X y la columna Y.
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			if((i != x) && (j != y))
				m3.push_back(e[i][j]);


	// Calculamos el signo que corresponde a este cofactor
	int sign = (((x+y) % 2) == 0) ? 1 : -1;

	// Calculamos el determinante
	float det = det3x3(
					m3[0], m3[1], m3[2],
					m3[3], m3[4], m3[5],
					m3[6], m3[7], m3[8]
				);

	// Evitamos el cero negativo
	if((det < 0.0f) || (det > 0.0f))
		det = sign * det;
	else
		det = 0.0f;

	return det;
}
