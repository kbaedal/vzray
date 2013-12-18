#include <cmath>
#include <ctime>
#include <cstdio>

#include "vec3.h"
#include "point.h"

#include "randomc/randomc.h"
#include "onb.h"

std::ostream& operator<<(std::ostream &os, const Vec3 &v)
{
	os << '<' << v.e[0] << ", " << v.e[1] << ", " << v.e[2] << '>';
	
	return os;
}

bool operator==(const Vec3 &v1, const Vec3 &v2)
{
	return ((v1.e[0] == v2.e[0]) && (v1.e[1] == v2.e[1]) && (v1.e[2] == v2.e[2]));
}

bool operator!=(const Vec3 &v1, const Vec3 &v2)
{
	return !(v1 == v2);
}

Vec3 operator+(const Vec3 &v1, const Vec3 &v2)
{
	return Vec3(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
}

Vec3 operator-(const Vec3 &v1, const Vec3 &v2)
{
	return Vec3(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}

Vec3 operator*(const Vec3 &v, float f)
{
	return Vec3(v.x() * f, v.y() * f, v.z() * f);
}

Vec3 operator*(float f, const Vec3 &v)
{
	return Vec3(v.x() * f, v.y() * f, v.z() * f);
}

Vec3 operator/(const Vec3 &v, float f)
{
	return Vec3(v.x() / f, v.y() / f, v.z() / f);
}

Vec3 operator/(float f, const Vec3 &v)
{
	return Vec3(v.x() / f, v.y() / f, v.z() / f);
}

void Vec3::set(const Point &p)
{
	e[0] = p.x();
	e[1] = p.y();
	e[2] = p.z();
}
		
float dot(Vec3 const &v1, Vec3 const &v2)
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

float tripleProduct(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3)
{
	return dot(cross(v1, v2), v3);
}

Vec3 versor(const Vec3 &v)
{
	return v / v.length();
}

Vec3 reflect(const Vec3 &v3In, const Vec3 &v3Norm)
{
	return Vec3(v3In - 2 * dot(v3In, v3Norm) * v3Norm);
}

Vec3 refract(const Vec3 &v3In, const Vec3 &v3Norm, const float dExtIOR, const float dIntIOR)
{
	float n = dExtIOR / dIntIOR;
	float cosI = -dot(v3Norm, v3In);
	float cosT2 = 1.0 - n * n * (1.0 - cosI * cosI);
	
	return Vec3((n * v3In) + (n * cosI - sqrt(cosT2)) * v3Norm);
}

Vec3 randomDir(const Vec3 &v3Dir, float dRN1, float dRN2)
{
	ONB uvw;
	
	uvw.initFromW(v3Dir);
	
	uvw.normalizeVectors();
	
	float two_pi = 6.28318530718f;
	float phi = two_pi * dRN1;
	float r = sqrt(dRN2);
	float x = r * cos(phi);
	float y = r * sin(phi);
	float z = sqrt(1 - x*x - y*y);
	
	return x*uvw.u() + y*uvw.v() + z*uvw.w();
}
