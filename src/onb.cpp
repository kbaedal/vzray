#include "onb.h"

const float ONB::konb_epsilon = 0.01f;

void ONB::init_from_u(const Vec3 &a_u)
{
	Vec3 n(1.0f, 0.0f, 0.0f);
	Vec3 m(0.0f, 1.0f, 0.0f);

	U = versor(a_u);

	V = cross(U, n);

	if(V.length() < konb_epsilon)
		V = cross(U, m);

	W = cross(U, V);
}

void ONB::init_from_v(const Vec3 &a_v)
{
	Vec3 n(1.0f, 0.0f, 0.0f);
	Vec3 m(0.0f, 1.0f, 0.0f);

	V = versor(a_v);

	U = cross(V, n);

	if(U.length() < konb_epsilon)
		U = cross(V, m);

	W = cross(V, U);
}

void ONB::init_from_w(const Vec3 &a_w)
{
	Vec3 n(1.0f, 0.0f, 0.0f);
	Vec3 m(0.0f, 1.0f, 0.0f);

	W = versor(a_w);

	U = cross(W, n);

	if(U.length() < konb_epsilon)
		U = cross(W, m);

	V = cross(W, U);
}

void ONB::init_from_uv(const Vec3 &a_u, const Vec3 &a_v)
{
	U = versor(a_u);

	W = versor(cross(a_u, a_v));

	V = cross(W, U);
}

void ONB::init_from_vu(const Vec3 &a_v, const Vec3 &a_u)
{
	V = versor(a_v);

	W = versor(cross(a_v, a_u));

	U = cross(W, V);
}

void ONB::init_from_uw(const Vec3 &a_u, const Vec3 &a_w)
{
	U = versor(a_u);

	V = versor(cross(a_w, a_u));

	W = cross(U, V);
}

void ONB::init_from_wu(const Vec3 &a_w, const Vec3 &a_u)
{
	W = versor(a_w);

	V = versor(cross(a_u, a_w));

	U = cross(W, V);
}

void ONB::init_from_vw(const Vec3 &a_v, const Vec3 &a_w)
{
	V = versor(a_v);

	U = versor(cross(a_w, a_v));

	W = cross(V, U);
}

void ONB::init_from_wv(const Vec3 &a_w, const Vec3 &a_v)
{
	W = versor(a_w);

	U = versor(cross(a_v, a_w));

	V = cross(W, U);
}

void ONB::normalize_vectors()
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
