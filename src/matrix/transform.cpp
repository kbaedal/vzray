#include <cstdlib>

#include "matrix/transform.h"

// Constantes utiles
const float PI 		= 3.14159265358979323846;
const float MINV 	= 0.00001;

// Para el qsort de la transformacion de bbox
int compareF(const void * a, const void * b)
{
	float f1 = *reinterpret_cast<const float*>(a);
    float f2 = *reinterpret_cast<const float*>(b);
    if(f1 < f2) return -1;
    if(f1 > f2) return 1;
    return 0;
}

// Actualiza la matriz y su inversa con los datos de otra transformacion.
void Transform::update(const Transform &a_tTrans)
{
	m_mtxM = a_tTrans.m_mtxM * m_mtxM;
	
	m_mtxI = m_mtxM.getInv();
}	

// Actualiza la matriz y su inversa con los datos de una matriz.
void Transform::update(const Matrix4x4 &a_mtxM)
{
	m_mtxM = a_mtxM * m_mtxM;
	m_mtxI = m_mtxM.getInv();
}
	
void Transform::translate(float dX, float dY, float dZ)
{
	Matrix4x4 mtxTemp;
	
    mtxTemp.e[0][3] = dX;
    mtxTemp.e[1][3] = dY;
    mtxTemp.e[2][3] = dZ;
	
    m_mtxM = mtxTemp * m_mtxM; 
    	
    m_mtxI = m_mtxM.getInv();
}

void Transform::scale(float dX, float dY, float dZ)
{
    Matrix4x4 mtxTemp;

    mtxTemp.e[0][0] = dX;
    mtxTemp.e[1][1] = dY;
    mtxTemp.e[2][2] = dZ;
    
    m_mtxM = mtxTemp * m_mtxM; 

    m_mtxI = m_mtxM.getInv();
}

void Transform::rotate(float dAng, const Vec3 &a_v3Axis)
{
	Matrix4x4 mtxTemp;
	
	Vec3 v3Axis = versor(a_v3Axis);
	float dSin = sin(dAng * (PI/180.f));
	float dCos = cos(dAng * (PI/180.f));
		
	if(dSin < MINV)
		dSin = 0.f;

	if(dCos < MINV)
		dCos = 0.f;
	
	mtxTemp.e[0][0] = dCos + v3Axis.x() * v3Axis.x() * (1.f - dCos);
	mtxTemp.e[0][1] = v3Axis.x() * v3Axis.y() * (1.f - dCos) - v3Axis.z() * dSin;
	mtxTemp.e[0][2] = v3Axis.x() * v3Axis.z() * (1.f - dCos) + v3Axis.y() * dSin;
	mtxTemp.e[0][3] = 0.f;
	
	mtxTemp.e[1][0] = v3Axis.x() * v3Axis.y() * (1.f - dCos) + v3Axis.z() * dSin;
	mtxTemp.e[1][1] = dCos + v3Axis.y() * v3Axis.y() * (1.f - dCos);
	mtxTemp.e[1][2] = v3Axis.y() * v3Axis.z() * (1.f - dCos) - v3Axis.x() * dSin;
	mtxTemp.e[1][3] = 0.f;
	
	mtxTemp.e[2][0] = v3Axis.x() * v3Axis.z() * (1.f - dCos) - v3Axis.y() * dSin;
	mtxTemp.e[2][1] = v3Axis.x() * v3Axis.z() * (1.f - dCos) + v3Axis.x() * dSin;
	mtxTemp.e[2][2] = dCos + v3Axis.z() * v3Axis.z() * (1.f - dCos);
	mtxTemp.e[2][3] = 0.f;
	
	mtxTemp.e[3][0] = 0.f;
	mtxTemp.e[3][1] = 0.f;
	mtxTemp.e[3][2] = 0.f;
	mtxTemp.e[3][3] = 1.f;	
	
	m_mtxM = mtxTemp * m_mtxM;

	m_mtxI = m_mtxM.getInv();
}

void Transform::rotateX(float dAng)
{
    Matrix4x4 mtxTemp;

    float dAngRad = dAng * (PI/180.0f);
    float dSinAng = sin(dAngRad);
    float dCosAng = cos(dAngRad);
	
	if(dSinAng < MINV)
		dSinAng = 0.f;

	if(dCosAng < MINV)
		dCosAng = 0.f;
 
    mtxTemp.e[1][1] = dCosAng;
    mtxTemp.e[1][2] = -dSinAng;
    mtxTemp.e[2][1] = dSinAng;
    mtxTemp.e[2][2] = dCosAng;
    
    m_mtxM = mtxTemp * m_mtxM;

    m_mtxI = m_mtxM.getInv();
}

void Transform::rotateY(float dAng)
{
    Matrix4x4 mtxTemp;

    float dAngRad = dAng * (PI/180.0f);
    float dSinAng = sin(dAngRad);
    float dCosAng = cos(dAngRad);
    
    if(dSinAng < MINV)
		dSinAng = 0.f;

	if(dCosAng < MINV)
		dCosAng = 0.f;

    mtxTemp.e[0][0] = dCosAng;
    mtxTemp.e[0][2] = dSinAng;
    mtxTemp.e[2][0] = -dSinAng;
    mtxTemp.e[2][2] = dCosAng;

    m_mtxM = mtxTemp * m_mtxM;

    m_mtxI = m_mtxM.getInv();
}

void Transform::rotateZ(float dAng)
{
    Matrix4x4 mtxTemp;

    float dAngRad = dAng * (PI/180.0f);
    float dSinAng = sin(dAngRad);
    float dCosAng = cos(dAngRad);
    
    if(dSinAng < MINV)
		dSinAng = 0.f;

	if(dCosAng < MINV)
		dCosAng = 0.f;

    mtxTemp.e[0][0] = dCosAng;
    mtxTemp.e[0][1] = -dSinAng;
    mtxTemp.e[1][0] = dSinAng;
    mtxTemp.e[1][1] = dCosAng;

    m_mtxM = mtxTemp * m_mtxM;

    m_mtxI = m_mtxM.getInv();
}

void Transform::freeTrans(const Matrix4x4 &mtxF)
{
    m_mtxM = m_mtxM * mtxF;

    m_mtxI = m_mtxM.getInv();
}

Ray Transform::sceneToObject(const Ray &rayIn)
{
	Ray rayTemp(
		m_mtxI.transform(rayIn.origin()),
		m_mtxI.transform(rayIn.direction())
	);
		
	rayTemp.refreshInv();
	
	return rayTemp;	
}

Point Transform::sceneToObject(const Point &p3Point)
{
	return m_mtxI.transform(p3Point);	
}

Vec3 Transform::objectNormalToScene(const Vec3 &v3Normal)
{
	return m_mtxI.getTrans().transform(v3Normal);
}

Ray Transform::objectToScene(const Ray &rayIn)
{
	Ray rayTemp(
		m_mtxM.transform(rayIn.origin()),
		m_mtxM.transform(rayIn.direction())
	);
	
	rayTemp.refreshInv();
	
	return rayTemp;	
}

Point Transform::objectToScene(const Point &p3Point)
{
	return m_mtxM.transform(p3Point);	
}

AABB Transform::updateAABB(const AABB &abBox)
{
	// A partir de los puntos min y max de la bbox tenemos que hallar
	// todos los puntos que la forman:
	//                                                      	
	//     7 --------- 8   Partiendo de los puntos que tenemos:
	//    /|          /|     1 = (a, b, c)  -> bbox.min
	//   / |         / |     8 = (d, e, f)  -> bbox.max                                    
	//  /  |        /  |   El resto son:
	// 3 --+------ 4   |     2 = (d, b, c)                           
	// |   5 ------+-- 6     3 = (a, e, c)                           
	// |  /        |  /      4 = (d, e, c)                           
	// | /         | /       5 = (a, b, f)                           
	// |/          |/        6 = (d, b, f)                           	
	// 1 --------- 2         7 = (a, e, f)                          
	//                                                      
	// Una vez calculados, les aplicamos la transformación y calculamos
	// cuales son los máximos y mínimos de cada coordenada. Devolvemos
	// una nueva bbox con estos valores.
	
	Point v[8];
	
	// Mínimo y máximo:	
	v[0] = abBox.m_p3Min;
	v[7] = abBox.m_p3Max;
	
	// Resto de puntos:
	v[1].set(v[7].x(), v[0].y(), v[0].z());
	v[2].set(v[0].x(), v[7].y(), v[0].z());
	v[3].set(v[7].x(), v[7].y(), v[0].z());
	v[4].set(v[0].x(), v[0].y(), v[7].z());
	v[5].set(v[7].x(), v[0].y(), v[7].z());
	v[6].set(v[0].x(), v[7].y(), v[7].z());
	
	// Transformamos:	
	for(int i = 0; i < 8; i++)
		v[i] = m_mtxM.transform(v[i]);
		
	// Crearemos tres arrays de floats. En cada uno metermos todas
	// las coordenadas de los puntos (en uno las x, en otro las y, etc)
	// y le aplicaremos el qsort de la biblioteca estandard. Asi tendremos
	// para cada uno de los arrays, el minimo en la primera posicion y 
	// el máximo en la ultima.
	float x[8], y[8], z[8];
	
	for(int i = 0; i < 8; i++) {
		x[i] = v[i].x();
		y[i] = v[i].y();
		z[i] = v[i].z();
	}
	
	std::qsort(x, 8, sizeof(float), compareF);
	std::qsort(y, 8, sizeof(float), compareF);
	std::qsort(z, 8, sizeof(float), compareF);
	
	// Valores ordenados, solo nos queda devolver la bbox
	return AABB(Point(x[0], y[0], z[0]), Point(x[7], y[7], z[7]));
}

std::ostream& operator<<(std::ostream &os, const Transform &t)
{
	os << "M:" << std::endl;
	os << t.m_mtxM << std::endl;
	os << "I:" << std::endl;
	os << t.m_mtxI << std::endl;
		
	return os;
}
