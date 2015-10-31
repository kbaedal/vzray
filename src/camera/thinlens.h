#ifndef __THINLENS_H__
#define __THINLENS_H__

#include "vec3.h"
#include "point.h"
#include "onb.h"
#include "ray.h"
#include "camera/camera.h"

/**
 * \class Definición de una cámara de lente fina (ThinLens Camera)
 */
class ThinLens : public Camera {
	public:
		ThinLens() {}
		ThinLens(Point a_center, Vec3 a_dir, Vec3 a_up, float a_aperture,
			float a_dist, float a_u0, float a_v0, float a_u1, float a_v1)
		{
			set(a_center, a_dir, a_up, a_aperture, a_dist, a_u0, a_v0, a_u1, a_v1);
		}

		void set(Point a_center, Vec3 a_dir, Vec3 a_up, float a_aperture,
			float a_dist, float a_u0, float a_v0, float a_u1, float a_v1);

		Ray get_ray(float x, float y, float sx, float sy);
	private:
		Point	center,
				corner;

		Vec3	dir,
				up,
				x_dir,
				y_dir;

		ONB 	uvw;

		float	lens_radius,	///< Radio de la lente.
				dist,
				u0,
				v0,
				u1,
				v1;
};

#endif // __THINLENS_H__
