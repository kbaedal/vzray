#include "camera/thinlens.h"

void ThinLens::set(Point a_center, Vec3 a_dir, Vec3 a_up, double a_aperture,
			double a_dist, double a_u0, double a_v0, double a_u1, double a_v1)
{
	center 		= a_center; // eye
	dir			= a_dir;	// gaze
	up			= a_up;		// up
	dist		= a_dist;	// s
	u0			= a_u0;
	v0			= a_v0;
	u1			= a_u1;
	v1			= a_v1;
	lens_radius	= a_aperture / 2.0f;

	uvw.init_from_wv(-dir, up);

	corner = center + u0 * Point(uvw.u()) + v0 * Point(uvw.v()) - dist * Point(uvw.w());

	x_dir = (u1 - u0) * uvw.u();
	y_dir = (v1 - v0) * uvw.v();
}

Ray ThinLens::get_ray(double x, double y, double sx, double sy)
{
	Point 	lens_point, img_plane;
	Vec3	ray_dir;

	// Calculamos un punto en la lente. Será el punto base del rayo.
	lens_point = center + 2.0f * (sx - 0.5f) * lens_radius * Point(uvw.u()) +
		2.0f * (sy - 0.5f) * lens_radius * Point(uvw.v());

	// Calculamos el punto correcto en el plano de la imagen.
	img_plane = corner + x * Point(x_dir) + y * Point(y_dir);

	// Obtenemos el vector del rayo.
	ray_dir = img_plane - lens_point;
	ray_dir.normalize();

	return Ray(lens_point, ray_dir);
}
