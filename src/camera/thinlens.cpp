#include "camera/thinlens.h"

void ThinLens::set(Point a_center, Vec3 a_dir, Vec3 a_up, float a_aperture,
			float a_dist, float a_u0, float a_v0, float a_u1, float a_v1)
{
	Point	u, v, w;	// Auxiliares para calculos.

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

	u.set(uvw.u());
	v.set(uvw.v());
	w.set(uvw.w());

	corner = center + u0 * u + v0 * v - dist * w;

	x_dir = (u1 - u0) * uvw.u();
	y_dir = (v1 - v0) * uvw.v();
}

Ray ThinLens::get_ray(float x, float y, float sx, float sy)
{
	Point 	lens_point, img_plane,
			t_udir, t_vdir, t_xdir, t_ydir;
	Vec3	ray_dir;

	// Auxiliares para los calculos
	t_udir.set(uvw.u());
	t_vdir.set(uvw.v());

	t_xdir.set(x_dir);
	t_ydir.set(y_dir);

	// Calculamos un punto en la lente. Será el punto base del rayo.
	lens_point = center + 2.0f * (sx - 0.5f) * lens_radius * t_udir +
		2.0f * (sy - 0.5f) * lens_radius * t_vdir;

	// Calculamos el punto correcto en el plano de la imagen.
	img_plane = corner + x * t_xdir + y * t_ydir;

	// Obtenemos el vector del rayo.
	ray_dir.set(img_plane - lens_point);
	ray_dir.normalize();

	return Ray(lens_point, ray_dir);
}
