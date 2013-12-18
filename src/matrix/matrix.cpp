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

void Matrix4x4::set(float dMatrix[4][4])
{
	for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
			e[i][j] = dMatrix[i][j];
}

void Matrix4x4::set(float dMatrix[16])
{	
	for(int i = 0, k = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
			e[i][j] = dMatrix[k++];
}

Matrix4x4 Matrix4x4::getInv()
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
	
	Matrix4x4 mtxTemp;
	
	// Calculamos el determinante. Si es cero, devolvemos la matriz identidad.
	float dDet = this->determinant();
	
	if(dDet != 0.f) {
		// Calculamos la matriz de cofactores
		mtxTemp = this->getCofactor();
						
		// La transponemos
		mtxTemp.transpose();
				
		// Y la dividimos por el determinante.
		mtxTemp = mtxTemp / dDet;
	}
	else {
		std::clog << "----\nMatriz SIN inversa posible.\n---\n" << std::endl;
		std::clog << *this << std::endl;
		std::clog << "Determinante: " << dDet << std::endl;
	}
		
	return mtxTemp;
}

Matrix4x4 Matrix4x4::getTrans()
{
	Matrix4x4 mtxTemp;
	
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            mtxTemp.e[i][j] = e[j][i];
    
    return mtxTemp;
}

Matrix4x4 Matrix4x4::getCofactor()
{
	Matrix4x4 mtxTemp;
	
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			mtxTemp.e[i][j] = cofactorXY(i, j);
		   
    return mtxTemp;
}

Matrix4x4 operator+(const Matrix4x4 &mtxM1, const Matrix4x4 &mtxM2)
{
	Matrix4x4 mtxT;
	
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            mtxT.e[i][j] = mtxM1.e[i][j] + mtxM2.e[i][j];

    return mtxT;
}

Matrix4x4 operator-(const Matrix4x4 &mtxM1, const Matrix4x4 &mtxM2)
{
	Matrix4x4 mtxT;
	
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            mtxT.e[i][j] = mtxM1.e[i][j] - mtxM2.e[i][j];

    return mtxT;
}

Matrix4x4 operator*(const Matrix4x4 &mtxM1, const Matrix4x4 &mtxM2)
{
    Matrix4x4 mtxTemp;

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            float dSum = 0.0;
            for(int k = 0; k < 4; k++)
				dSum += mtxM1.e[i][k] * mtxM2.e[k][j];
           
            mtxTemp.e[i][j] = dSum;
        }
    }

    return mtxTemp;
}

Matrix4x4 operator/(const Matrix4x4 &mtxM1, const Matrix4x4 &mtxM2)
{
    Matrix4x4 mtxRes, mtxTemp;
    
    mtxTemp = mtxM2;
    mtxTemp.invert();

    mtxRes = mtxM1 * mtxTemp;

    return mtxRes;
}

Vec3 Matrix4x4::transform(const Vec3 &v3Vector)
{
	float dVector[4], dNVec[4];
    
    dVector[0] = v3Vector.x();
    dVector[1] = v3Vector.y();
    dVector[2] = v3Vector.z();
    dVector[3] = 0.0;

    for(int i = 0; i < 4; i++) {
        float dTemp = 0.0;
        
        for(int j = 0; j < 4; j++)
            dTemp += e[i][j] * dVector[j];
        
        if((dTemp > 0.0) || (dTemp < 0.0))
            dNVec[i] = dTemp;
        else
            dNVec[i] = 0.0f;
    }

    return Vec3(dNVec[0], dNVec[1], dNVec[2]);
}
       
Point Matrix4x4::transform(const Point &p3Point)
{
	float dPoint[4], dN_P[4];
    
    dPoint[0] = p3Point.x();
    dPoint[1] = p3Point.y();
    dPoint[2] = p3Point.z();
    dPoint[3] = 1.0;

    for(int i = 0; i < 4; i++) {
        float dTemp = 0.0;
        
        for(int j = 0; j < 4; j++) {
            dTemp += e[i][j] * dPoint[j];
        }

        if((dTemp > 0.0) or (dTemp < 0.0))
            dN_P[i] = dTemp;
        else
            dN_P[i] = 0.0f;
    }

    return Point(dN_P[0], dN_P[1], dN_P[2]);
}

Matrix4x4 operator*(const Matrix4x4 &mtxMatrix, float dV)
{
	Matrix4x4 mtxTemp;	
	
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			mtxTemp.e[i][j] = mtxMatrix.e[i][j] * dV;
		
	return mtxTemp;
}

Matrix4x4 operator/(const Matrix4x4 &mtxMatrix, float dV)
{
	Matrix4x4 mtxTemp;	
	
	if(dV != 0.0) {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				mtxTemp.e[i][j] = mtxMatrix.e[i][j] / dV;
	}		
	
	return mtxTemp;
}

float Matrix4x4::determinant()
{
	float dDet = 0.f;
	
	// La funcion cofactorXY ya devuelve el signo adecuado para la
	// posición calculada, con lo que simplemente tenemos que sumar.
	dDet += e[0][0] * cofactorXY(0, 0);
	dDet += e[0][1] * cofactorXY(0, 1);
	dDet += e[0][2] * cofactorXY(0, 2);
	dDet += e[0][3] * cofactorXY(0, 3);
	
	return dDet;
}

float det3(
	float a11, float a12, float a13,
	float a21, float a22, float a23,
	float a31, float a32, float a33
)
{
	// Aplicamos la regla de Sarrus
	float dDet = 0.f;
	
	dDet += a11 * a22 * a33;
	dDet += a12 * a23 * a31;
	dDet += a13 * a21 * a32;
	
	dDet -= a13 * a22 * a31;
	dDet -= a12 * a21 * a33;
	dDet -= a11 * a23 * a32;
	
	return dDet;
}

float Matrix4x4::cofactorXY(int X, int Y)
{
	std::vector<float> m3;
	
	// Almacenamos por orden los 9 numeros resultantes de eliminar
	// de la matriz de 4x4 la fila X y la columna Y. 
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			if((i != X) && (j != Y))
				m3.push_back(e[i][j]);				
	
	
	// Calculamos el signo que corresponde a este cofactor
	int nSign = (((X+Y) % 2) == 0) ? 1 : -1;
	
	// Calculamos el determinante
	float dDet = det3(
					m3[0], m3[1], m3[2], 
					m3[3], m3[4], m3[5], 
					m3[6], m3[7], m3[8]
				);
		
	// Evitamos el cero negativo
	if((dDet < 0.f) || (dDet > 0.f))
		dDet = nSign * dDet;
	else
		dDet = 0.f;
	
	return dDet;
}
