#include "ray.h"

Point Ray::get_point(const double dist) const
{
	return Point(orig + dist * Point(dir));
}

void Ray::refresh_inv()
{
	// Inverted direction and signs, for ray-box tests
	i_dir.set(1.f/dir.x(), 1.f/dir.y(), 1.f/dir.z());

	// Sign could be 0 or 1 (int values for false and true)
	sign[0] = (i_dir.x() < 0.f);
	sign[1] = (i_dir.y() < 0.f);
	sign[2] = (i_dir.z() < 0.f);
}

std::ostream& operator<<(std::ostream &os, const Ray &r)
{
	os << "(" << r.origin() << ") + t(" << r.direction() << ")";

	return os;
}
