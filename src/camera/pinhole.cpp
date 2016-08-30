#include "camera/pinhole.h"

void Pinhole::set(Point a_pos, Vec3 a_dir, Vec3 a_up,
			double a_dist, double a_u0, double a_v0, double a_u1, double a_v1, double a_push)
{
	Point	u, v, w;	// Auxiliares para calculos.

	pos     = a_pos;    // eye
	dir		= a_dir;	// gaze
	up		= a_up;		// up
	dist	= a_dist;   // s
	u0		= a_u0;
	v0		= a_v0;
	u1		= a_u1;
	v1		= a_v1;

	uvw.init_from_wv(-dir, up);

	u = uvw.u();
	v = uvw.v();
	w = uvw.w();

	//m_p3Corner = pos + m_u0 * m_uvw.u() + m_v0 * m_uvw.v() - m_dDist * m_uvw.w();
	//corner = pos + u0 * u + v0 * v - dist * w;
	corner = pos + u0 * Point(uvw.u()) + v0 * Point(uvw.v()) - dist * Point(uvw.w());

	x_dir = (u1 - u0) * uvw.u();
	y_dir = (v1 - v0) * uvw.v();

	push = a_push;
}

Ray Pinhole::get_ray(double x, double y, double sx, double sy)
{
    if((x < -.5f) || (y < -.5f))
        std::clog << "Bad ray (" << x << ", " << y << ")\n";

    if((sx < 0.0f) || (sx > 1.0f) || (sy < 0.0f) || (sy > 1.0f))
        std::clog << "Bad sub-ray (" << sx << ", " << sy << ")\n";

	Point 	img_plane;  // Destino en el plano de imagen del rayo.

	Vec3	ray_dir;

	img_plane = corner + x * Point(x_dir) + y * Point(y_dir);

	ray_dir = img_plane - pos;
	ray_dir.normalize();

	return Ray(pos + push * Point(ray_dir), ray_dir);
}
