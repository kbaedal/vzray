#include "camera/pinhole.h"

void Pinhole::set(Point a_pos, Vec3 a_dir, Vec3 a_up,
			float a_dist, float a_u0, float a_v0, float a_u1, float a_v1, float a_push)
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

	u.set(uvw.u());
	v.set(uvw.v());
	w.set(uvw.w());

	//m_p3Corner = pos + m_u0 * m_uvw.u() + m_v0 * m_uvw.v() - m_dDist * m_uvw.w();
	corner = pos + u0 * u + v0 * v - dist * w;

	x_dir = (u1 - u0) * uvw.u();
	y_dir = (v1 - v0) * uvw.v();

	push = a_push;
}

Ray Pinhole::get_ray(float x, float y, float sx, float sy)
{
    if((x < -.5f) || (y < -.5f))
        std::clog << "Bad ray (" << x << ", " << y << ")\n";

    if((sx < 0.0f) || (sx > 1.0f) || (sy < 0.0f) || (sy > 1.0f))
        std::clog << "Bad sub-ray (" << sx << ", " << sy << ")\n";

	Point 	img_plane,                  // Destino en el plano de imagen del rayo.
            t_xdir, t_ydir, t_pushdir;  // Convertiremos los vectores a puntos.
	Vec3	ray_dir;

	t_xdir.set(x_dir);
	t_ydir.set(y_dir);
	img_plane = corner + x * t_xdir + y * t_ydir;

	ray_dir.set(img_plane - pos);
	ray_dir.normalize();
	t_pushdir.set(ray_dir);

	return Ray(pos + push * t_pushdir, ray_dir);
}
