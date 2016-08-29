#include <cmath>
#include <ctime>
#include <cstdio>

#include "vec3.h"
#include "point.h"

#include "randomc/randomc.h"
#include "onb.h"

Vec3::Vec3(const Point &p)
{
    e[0] = p.x();
    e[1] = p.y();
    e[2] = p.z();
}

std::ostream &operator<<(std::ostream &os, const Vec3 &v)
{
	os << "Vec3<" << v.e[0] << ", " << v.e[1] << ", " << v.e[2] << ">";

	return os;
}

Vec3 &Vec3::operator+=(Vec3 const &v)
{
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];

    return *this;
}

bool operator==(const Vec3 &v1, const Vec3 &v2)
{
	return ((v1.e[0] == v2.e[0]) && (v1.e[1] == v2.e[1]) && (v1.e[2] == v2.e[2]));
}

bool operator!=(const Vec3 &v1, const Vec3 &v2)
{
	return !(v1 == v2);
}

Vec3 operator+(Vec3 v1, const Vec3 &v2)
{
	return Vec3(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
}

Vec3 operator-(Vec3 v1, const Vec3 &v2)
{
	return Vec3(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}

Vec3 operator*(Vec3 v, double f)
{
	return Vec3(v.x() * f, v.y() * f, v.z() * f);
}

Vec3 operator*(double f, Vec3 v)
{
	return Vec3(v.x() * f, v.y() * f, v.z() * f);
}

Vec3 operator/(Vec3 v, double f)
{
    double inv = 1.0f / f;

	return Vec3(v.x() * inv, v.y() * inv, v.z() * inv);
}

void Vec3::set(const Point &p)
{
	e[0] = p.x();
	e[1] = p.y();
	e[2] = p.z();
}

double dot(Vec3 const &v1, Vec3 const &v2)
{
	return (v1.e[0]*v2.e[0] + v1.e[1]*v2.e[1] + v1.e[2]*v2.e[2]);
}

Vec3 cross(Vec3 const &v1, Vec3 const &v2)
{
	return Vec3(
		v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1],
		v1.e[2]*v2.e[0] - v1.e[0]*v2.e[2],
		v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]
	);
}

double triple_product(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3)
{
	return dot(cross(v1, v2), v3);
}

Vec3 versor(const Vec3 &v)
{
	return v / v.length();
}

Vec3 reflect(const Vec3 &in, const Vec3 &norm)
{
	return Vec3(in - 2.0f * dot(in, norm) * norm);
}

Vec3 refract(const Vec3 &in, const Vec3 &norm, const double ext_ior, const double int_ior)
{
	double n = ext_ior / int_ior;
	double cos_i = -dot(norm, in);
	double cos_t2 = 1.0f - n * n * (1.0f - cos_i * cos_i);

	return Vec3((n * in) + (n * cos_i - sqrt(cos_t2)) * norm);
}

bool tir(const Vec3 &in, const Vec3 &norm, const double ior)
{
    double cos_t2;			// Aux for TIR calculation

	if(dot(in, norm) < 0.0f) {
		return false;
	}
	else {
		double t = -(dot(in, -norm));
		cos_t2 = 1.0f - (ior * ior) * (1.0f - t * t);

		if(cos_t2 < 0.0f)
		{
			return true;
		}
	}
	return false;
}

Vec3 random_dir(const Vec3 &dir, double rn1, double rn2)
{
    ONB uvw;

	uvw.init_from_w(dir);

	uvw.normalize_vectors();

	double two_pi = 6.28318530718f;
	double phi = two_pi * rn1;
	double r = sqrt(rn2);
	double x = r * cos(phi);
	double y = r * sin(phi);

	double z = sqrt(1.0f - x*x - y*y);

	/*
	double m = 10.0f;

    double two_pi = 6.28318530718f;
    double theta = two_pi * rn1;
    double phi = acos(pow((1.0f - rn2), 1.0f/(m + 1.0f)));

    double x = sin(theta) * cos(phi);
    double y = sin(theta) * cos(theta);
    double z = cos(theta);
    */

    return x*uvw.u() + y*uvw.v() + z*uvw.w();
}
