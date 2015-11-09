#include "isecaux.h"
#include "statistics.h"

bool isecaux::test_ray_box(
	const Ray   &r, 		    // Ray to test
	const Point &minimo,	    // Box min and max
	const Point &maximo,
	double      min_dist, 		// Min and Max distance
	double      max_dist,
	double      &dist)
{
    ++Statistics::num_prim_tests;

	double t1 = (minimo.x() - r.origin().x()) * r.inv_dir().x();
	double t2 = (maximo.x() - r.origin().x()) * r.inv_dir().x();
	double t3 = (minimo.y() - r.origin().y()) * r.inv_dir().y();
	double t4 = (maximo.y() - r.origin().y()) * r.inv_dir().y();
	double t5 = (minimo.z() - r.origin().z()) * r.inv_dir().z();
	double t6 = (maximo.z() - r.origin().z()) * r.inv_dir().z();

	double tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	double tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	// if tmax < 0, ray (line) is intersecting the box, but the whole box is behind us
	if (tmax < 0.0f)
		return false;

	// if tmin > tmax, ray doesn't intersect box
	if (tmin > tmax)
		return false;

	// Hit out of range.
	if((tmin < min_dist) || (tmin > max_dist))
		return false;

	dist = tmin;

	++Statistics::num_prim_isecs;

	return true;
}

Vec3 isecaux::get_box_normal(Point p)
{
	// Se asume que el cubo tiene coordenadas min<-1, -1, -1> max<1, 1, 1>
	// y que el punto está en alguna de las caras del cubo.

	// Base de vectores
	Vec3 n[3] = { Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f) };

	int coord;

	if((fabs(p.x()) > fabs(p.y())) && (fabs(p.x()) > fabs(p.z())))
		coord = 0;
	else if((fabs(p.y()) > fabs(p.x())) && (fabs(p.y()) > fabs(p.z())))
		coord = 1;
	else if((fabs(p.z()) > fabs(p.y())) && (fabs(p.z()) > fabs(p.x())))
		coord = 2;

	return (p.e[coord] > 0) ? n[coord] : -1 * n[coord];
};

bool isecaux::solve_quadratic(double a, double b, double c, double &t0, double &t1)
{
	double discr = b * b - 4.f * a * c;

	if (discr <= 0.0f) // Sin soluciones
		return false;

	double raiz_discr 	= sqrtf(discr);
	double divisor		= 1.0f / (2.0f * a);

	// Calculamos las raices
	t0 = (-b - raiz_discr) * divisor;
	t1 = (-b + raiz_discr) * divisor;

	// Intercambiamos si es necesario
	if(t0 > t1)
		std::swap(t0, t1);

	return true;
}

bool isecaux::test_ray_sphere(
	const Ray   &r,
	double      min_dist,
	double      max_dist,
	double 		&dist)
{
    ++Statistics::num_prim_tests;

    Vec3 o(r.origin());

	// Se asume que la esfera tiene su centro en el origen (0,0,0) y que
	// su radio es 1.
	double a = dot(r.direction(), r.direction());
	double b = 2.0f * dot(r.direction(), o);

	// En el espacio de la esfera, el radio es 1 siempre.
	double c = dot(o, o) - 1.0f;

	double t0, t1;

	if(!isecaux::solve_quadratic(a, b, c, t0, t1))
		return false;

	if(t0 < min_dist)
		dist = t1;
	else
		dist = t0;

	if((dist < min_dist) || (dist > max_dist))
		return false;

    ++Statistics::num_prim_isecs;

	return true;
}

bool isecaux::test_ray_triangle(
	const Ray   &r,	 		// Ray to test
	const Point &p0, 		// Triangle vertexs
	const Point &p1,
	const Point &p2,
	double      min_dist,   // Min and Max distance
	double      max_dist,
	double      &dist)		// If hit, stores distance.
{
    ++Statistics::num_triangle_tests;

	double a = p0.x() - p1.x();
	double b = p0.y() - p1.y();
	double c = p0.z() - p1.z();

	double d = p0.x() - p2.x();
	double e = p0.y() - p2.y();
	double f = p0.z() - p2.z();

	double g = r.direction().x();
	double h = r.direction().y();
	double i = r.direction().z();

	double j = p0.x() - r.origin().x();
	double k = p0.y() - r.origin().y();
	double l = p0.z() - r.origin().z();

	double eihf = e*i-h*f;
	double gfdi = g*f-d*i;
	double dheg = d*h-e*g;

	double denom = (a*eihf + b*gfdi + c*dheg);

	double beta = (j*eihf + k*gfdi + l*dheg) / denom;

	if (beta <= 0.0f || beta >= 1.0f) return false;

	double akjb = a*k - j*b;
	double jcal = j*c - a*l;
	double blkc = b*l - k*c;

	double gamma = (i*akjb + h*jcal + g*blkc) / denom;
	if(gamma <= 0.0f || beta+gamma >= 1.0f) return false;

	double temp = -(f*akjb + e*jcal + d*blkc) / denom;
	if (temp >= min_dist && temp <= max_dist) {
		dist = temp;

		++Statistics::num_triangles_isec;

		return true;
	}

	return false;
}

bool isecaux::test_ray_cylinder(
	const Ray   &r,
	double      min_dist,
	double      max_dist,
	double 		&dist)
{
    ++Statistics::num_prim_tests;

	// Se asume que el cilindro está situado a lo largo del eje Z,
	// y que sus extremos son <0, 0, -1> y <0, 0, 1>

	double a = r.direction().x() * r.direction().x() + r.direction().y() * r.direction().y();
	double b = 2.0f * (r.direction().x() * r.origin().x() + r.direction().y() * r.origin().y());

	// Radio del cilindro = 1
	double c = r.origin().x() * r.origin().x() + r.origin().y() * r.origin().y() - 1.0f;

	double t0, t1;

	if(!isecaux::solve_quadratic(a, b, c, t0, t1))
		return false;

	if(t0 < min_dist) {
		// El primer punto de interseccion está tras la cámara
		dist = t1;

		// Comprobamos el segundo.
		if((dist < min_dist) || (dist > max_dist))
			return false;

		// Ahora comprobaremos si está dentro del cilindro o fuera
		Point p(r.get_point(dist));

		if((p.z() < -1.0f) || (p.z() > 1.0f))
			return false;
	}
	else {
		// Tenemos los dos puntos por delante de la camara, habrá
		// que comprobarlos los dos.
		dist = t0;

		if(dist > max_dist)
			return false;

		Point p(r.get_point(dist));

		// Si dT0 no está dentro de los márgenes, puede que dT1 sí.
		if((p.z() < -1.0f) || (p.z() > 1.0f)) {
			dist = t1;

			if(dist > max_dist)
				return false;

			p = r.get_point(dist);

			if((p.z() < -1.0f) || (p.z() > 1.0f))
				return false;
		}
	}

    ++Statistics::num_prim_isecs;

	return true;
}

bool isecaux::test_ray_plane(
    const Ray   &r,
    const Point &base,
    const Vec3  &normal,
    double      min_dist,
    double      max_dist,
    double      &dist)
{
    ++Statistics::num_prim_tests;

    double  denom,
            num,
            t;

    denom = dot(r.direction(), normal);
    if(denom != 0.0f) {
        num = dot(Vec3(base - r.origin()), normal);
        t   = num/denom;

        if((t > 0.0f) && (t > min_dist) && (t < max_dist))  { // Hit.
            ++Statistics::num_prim_isecs;

            dist = t;

            return true;
        }
        else {
            return false;
        }
    }

    return false;
}

bool isecaux::test_ray_disc(
    const Ray   &r,
    const Point &center,
    const Vec3  &normal,
    double      radius,
    double      min_dist,
    double      max_dist,
    double      &dist)
{
    ++Statistics::num_prim_tests;

    double  denom,
            num,
            t;

    denom = dot(r.direction(), normal);
    if(denom != 0.0f) {
        num = dot(Vec3(center - r.origin()), normal);
        t   = num/denom;

        if((t > 0.0f) && (t > min_dist) && (t < max_dist))  { // Hit en el plano.
            Point   p(r.get_point(t));
            Vec3    vp(p - center);

            if(vp.length() <= radius) { // Hit en el disco.
                ++Statistics::num_prim_isecs;

                dist = t;

                return true;
            }
            else {
                return false;
            }
        }
    }

    return false;
}

