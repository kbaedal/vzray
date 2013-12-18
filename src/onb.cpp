#include "onb.h"

const float ONB_EPSILON = 0.01f;

void ONB::initFromU(const Vec3 &a_u)
{
	Vec3 n(1.0f, 0.0f, 0.0f);
	Vec3 m(0.0f, 1.0f, 0.0f);
	
	U = versor(a_u);
		
	V = cross(U, n);
	
	if(V.length() < ONB_EPSILON)
		V = cross(U, m);
	
	W = cross(U, V);
}

void ONB::initFromV(const Vec3 &a_v)
{
	Vec3 n(1.0f, 0.0f, 0.0f);
	Vec3 m(0.0f, 1.0f, 0.0f);
	
	V = versor(a_v);
		
	U = cross(V, n);
	
	if(U.length() < ONB_EPSILON)
		U = cross(V, m);
	
	W = cross(V, U);
}

void ONB::initFromW(const Vec3 &a_w)
{
	Vec3 n(1.0f, 0.0f, 0.0f);
	Vec3 m(0.0f, 1.0f, 0.0f);
	
	W = versor(a_w);
		
	U = cross(W, n);
	
	if(U.length() < ONB_EPSILON)
		U = cross(W, m);
	
	V = cross(W, U);
}

void ONB::initFromUV(const Vec3 &a_u, const Vec3 &a_v)
{
	U = versor(a_u);
		
	W = versor(cross(a_u, a_v));
		
	V = cross(W, U);
}

void ONB::initFromVU(const Vec3 &a_v, const Vec3 &a_u)
{
	V = versor(a_v);
		
	W = versor(cross(a_v, a_u));
		
	U = cross(W, V);
}

void ONB::initFromUW(const Vec3 &a_u, const Vec3 &a_w)
{
	U = versor(a_u);
		
	V = versor(cross(a_w, a_u));
		
	W = cross(U, V);
}

void ONB::initFromWU(const Vec3 &a_w, const Vec3 &a_u)
{
	W = versor(a_w);
		
	V = versor(cross(a_u, a_w));
	
	U = cross(W, V);
}

void ONB::initFromVW(const Vec3 &a_v, const Vec3 &a_w)
{
	V = versor(a_v);
		
	U = versor(cross(a_w, a_v));
		
	W = cross(V, U);
}

void ONB::initFromWV(const Vec3 &a_w, const Vec3 &a_v)
{
	W = versor(a_w);
		
	U = versor(cross(a_v, a_w));
		
	V = cross(W, U);
}

void ONB::normalizeVectors()
{
	U.normalize();
	V.normalize();
	W.normalize();
}

bool operator==(const ONB &o1, const ONB &o2)
{
	return ((o1.u() == o2.u()) && (o1.v() == o2.v()) && (o1.w() == o2.w()));
}

bool operator!=(const ONB &o1, const ONB &o2)
{
	return !(o1 == o2);
}

std::ostream &operator<<(std::ostream &os, const ONB &onb)
{
	os << onb.u() << "\n" << onb.v() << "\n" << onb.w() << "\n";
	
	return os;
}
