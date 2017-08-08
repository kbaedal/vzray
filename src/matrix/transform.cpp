#include <cstdlib>

#include "matrix/transform.h"

#include "log.h"

const double Transform::kpi          = 3.14159265358979323846;
const double Transform::kmin_value   = 0.00001;

int compare_float(const void * a, const void * b)
{
	double f1 = *reinterpret_cast<const float*>(a);
    double f2 = *reinterpret_cast<const float*>(b);
    if(f1 < f2) return -1;
    if(f1 > f2) return 1;
    return 0;
}

// Actualiza la matriz y su inversa con los datos de otra transformacion.
void Transform::update(const Transform &t)
{
	mtx = t.mtx * mtx;

	inv = mtx.get_inv();
}

// Actualiza la matriz y su inversa con los datos de una matriz.
void Transform::update(const Matrix4x4 &m)
{
	mtx = m * mtx;
	inv = mtx.get_inv();
}

void Transform::translate(double x, double y, double z)
{
	Matrix4x4 mtemp;

    mtemp.e[0][3] = x;
    mtemp.e[1][3] = y;
    mtemp.e[2][3] = z;

    mtx = mtemp * mtx;

    inv = mtx.get_inv();
}

void Transform::scale(double x, double y, double z)
{
    Matrix4x4 mtemp;

    mtemp.e[0][0] = x;
    mtemp.e[1][1] = y;
    mtemp.e[2][2] = z;

    mtx = mtemp * mtx;

    inv = mtx.get_inv();
}

void Transform::rotate(double angle, const Vec3 &axis)
{
	Matrix4x4 mtemp;

	Vec3 vtemp = versor(axis);
	double sin_a = sin(angle * (kpi/180.f));
	double cos_a = cos(angle * (kpi/180.f));

	if(sin_a < kmin_value)
		sin_a = 0.0f;

	if(cos_a < kmin_value)
		cos_a = 0.0f;

	mtemp.e[0][0] = cos_a + vtemp.x() * vtemp.x() * (1.0f - cos_a);
	mtemp.e[0][1] = vtemp.x() * vtemp.y() * (1.0f - cos_a) - vtemp.z() * sin_a;
	mtemp.e[0][2] = vtemp.x() * vtemp.z() * (1.0f - cos_a) + vtemp.y() * sin_a;
	mtemp.e[0][3] = 0.0f;

	mtemp.e[1][0] = vtemp.x() * vtemp.y() * (1.0f - cos_a) + vtemp.z() * sin_a;
	mtemp.e[1][1] = cos_a + vtemp.y() * vtemp.y() * (1.0f - cos_a);
	mtemp.e[1][2] = vtemp.y() * vtemp.z() * (1.0f - cos_a) - vtemp.x() * sin_a;
	mtemp.e[1][3] = 0.0f;

	mtemp.e[2][0] = vtemp.x() * vtemp.z() * (1.0f - cos_a) - vtemp.y() * sin_a;
	mtemp.e[2][1] = vtemp.x() * vtemp.z() * (1.0f - cos_a) + vtemp.x() * sin_a;
	mtemp.e[2][2] = cos_a + vtemp.z() * vtemp.z() * (1.0f - cos_a);
	mtemp.e[2][3] = 0.0f;

	mtemp.e[3][0] = 0.0f;
	mtemp.e[3][1] = 0.0f;
	mtemp.e[3][2] = 0.0f;
	mtemp.e[3][3] = 1.0f;

	mtx = mtemp * mtx;

	inv = mtx.get_inv();
}

void Transform::rotate_x(double angle)
{
    Matrix4x4 mtemp;

    double rad_a = angle * (kpi/180.0f);
    double sin_a = sin(rad_a);
    double cos_a = cos(rad_a);

	if(sin_a < kmin_value)
		sin_a = 0.0f;

	if(cos_a < kmin_value)
		cos_a = 0.0f;

    mtemp.e[1][1] = cos_a;
    mtemp.e[1][2] = -sin_a;
    mtemp.e[2][1] = sin_a;
    mtemp.e[2][2] = cos_a;

    mtx = mtemp * mtx;

    inv = mtx.get_inv();
}

void Transform::rotate_y(double angle)
{
    Matrix4x4 mtemp;

    double rad_a = angle * (kpi/180.0f);
    double sin_a = sin(rad_a);
    double cos_a = cos(rad_a);

	if(sin_a < kmin_value)
		sin_a = 0.0f;

	if(cos_a < kmin_value)
		cos_a = 0.0f;

    mtemp.e[0][0] = cos_a;
    mtemp.e[0][2] = sin_a;
    mtemp.e[2][0] = -sin_a;
    mtemp.e[2][2] = cos_a;

    mtx = mtemp * mtx;

    inv = mtx.get_inv();
}

void Transform::rotate_z(double angle)
{
    Matrix4x4 mtemp;

    double rad_a = angle * (kpi/180.0f);
    double sin_a = sin(rad_a);
    double cos_a = cos(rad_a);

	if(sin_a < kmin_value)
		sin_a = 0.0f;

	if(cos_a < kmin_value)
		cos_a = 0.0f;

    mtemp.e[0][0] = cos_a;
    mtemp.e[0][1] = -sin_a;
    mtemp.e[1][0] = sin_a;
    mtemp.e[1][1] = cos_a;

    mtx = mtemp * mtx;

    inv = mtx.get_inv();
}

void Transform::free_transform(const Matrix4x4 &m)
{
    mtx = mtx * m;

    inv = mtx.get_inv();
}

Ray Transform::scene_to_object(const Ray &r)
{
    /*
	Ray t(
		inv.transform(r.origin()),
		inv.transform(r.direction())
	);
	*/

	Ray t(
       inv.transform_pos(r.origin()),
       inv.transform_vec(r.direction())
    );

	t.refresh_inv();

	return t;
}

Point Transform::scene_to_object(const Point &p)
{
	//return inv.transform(p);
	return inv.transform_pos(p);
}

Vec3 Transform::normal_to_scene(const Vec3 &n)
{
	//return inv.get_trans().transform(n);
	return inv.get_trans().transform_vec(n);
}

Ray Transform::object_to_scene(const Ray &r)
{
    /*
	Ray t(
		mtx.transform(r.origin()),
		mtx.transform(r.direction())
	);
	*/
	Ray t(
		mtx.transform_pos(r.origin()),
		mtx.transform_vec(r.direction())
	);

	t.refresh_inv();

	return t;
}

Point Transform::object_to_scene(const Point &p)
{
	//return mtx.transform(p);
	return mtx.transform_pos(p);
}

AABB Transform::update_AABB(const AABB &b)
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
	v[0] = b.minimo;
	v[7] = b.maximo;

	// Resto de puntos:
	v[1].set(v[7].x(), v[0].y(), v[0].z());
	v[2].set(v[0].x(), v[7].y(), v[0].z());
	v[3].set(v[7].x(), v[7].y(), v[0].z());
	v[4].set(v[0].x(), v[0].y(), v[7].z());
	v[5].set(v[7].x(), v[0].y(), v[7].z());
	v[6].set(v[0].x(), v[7].y(), v[7].z());

	// Transformamos:
	for(int i = 0; i < 8; i++)
		//v[i] = mtx.transform(v[i]);
		v[i] = mtx.transform_pos(v[i]);

	// Crearemos tres arrays de floats. En cada uno metermos todas
	// las coordenadas de los puntos (en uno las x, en otro las y, etc.)
	// y le aplicaremos el qsort de la biblioteca estandard. Asi tendremos
	// para cada uno de los arrays, el minimo en la primera posicion y
	// el máximo en la ultima.
	double x[8], y[8], z[8];

	for(int i = 0; i < 8; i++) {
		x[i] = v[i].x();
		y[i] = v[i].y();
		z[i] = v[i].z();
		std::clog << v[i] << std::endl;
	}

	Point minp(x[0], y[0], z[0]);
	Point maxp(x[0], y[0], z[0]);
	std::clog << "Min-Max" << minp << maxp << std::endl;
	for(int i = 1; i < 8; i++) {
        if(x[i] < minp.x())
            minp.e[0] = x[i];
        if(x[i] > maxp.x())
            maxp.e[0] = x[i];

        if(y[i] < minp.y())
            minp.e[1] = y[i];
        if(y[i] > maxp.y())
            maxp.e[1] = y[i];

        if(z[i] < minp.z())
            minp.e[2] = z[i];
        if(z[i] > maxp.z())
            maxp.e[2] = z[i];
	}
	std::clog << "Min-Max" << minp << maxp << std::endl;

	//std::qsort(x, 8, sizeof(double), compare_float);
	//std::qsort(y, 8, sizeof(double), compare_float);
	//std::qsort(z, 8, sizeof(double), compare_float);

	// Valores ordenados, solo nos queda devolver la bbox
	//return AABB(Point(x[0], y[0], z[0]), Point(x[7], y[7], z[7]));
	return AABB(minp, maxp);
}

std::ostream& operator<<(std::ostream &os, const Transform &t)
{
	os << "M:" << std::endl;
	os << t.mtx << std::endl;
	os << "I:" << std::endl;
	os << t.inv << std::endl;

	return os;
}
