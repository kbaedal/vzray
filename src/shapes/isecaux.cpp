#include "isecaux.h"

bool testRayBox(
	const Ray &r, 		// Ray to test
	const Point &min,	// Box max and min
	const Point &max,  
	float dMin, 		// Max and Min distance
	float dMax, 
	float &dTVal)		 
{
	float t1 = (min.x() - r.origin().x()) * r.inv_dir().x();
	float t2 = (max.x() - r.origin().x()) * r.inv_dir().x();
	float t3 = (min.y() - r.origin().y()) * r.inv_dir().y();
	float t4 = (max.y() - r.origin().y()) * r.inv_dir().y();
	float t5 = (min.z() - r.origin().z()) * r.inv_dir().z();
	float t6 = (max.z() - r.origin().z()) * r.inv_dir().z();

	float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	// if tmax < 0, ray (line) is intersecting the box, but the whole box is behind us
	if (tmax < 0.f)
		return false;
	
	// if tmin > tmax, ray doesn't intersect box
	if (tmin > tmax)
		return false;
	
	// Hit out of range.
	if((tmin < dMin) || (tmin > dMax))
		return false;
		
	dTVal = tmin;
	return true;
}

Vec3 getBoxNormal(Point p3Point)
{	
	// Se asume que el cubo tiene coordenadas min<-1, -1, -1> max<1, 1, 1>
	// y que el punto está en alguna de las caras del cubo.
	
	// Base de vectores
	Vec3 n[3] = { Vec3(1.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f) };
	
	int nCoord;
	
	if((fabs(p3Point.x()) > fabs(p3Point.y())) && (fabs(p3Point.x()) > fabs(p3Point.z())))
		nCoord = 0;
	else if((fabs(p3Point.y()) > fabs(p3Point.x())) && (fabs(p3Point.y()) > fabs(p3Point.z())))
		nCoord = 1;
	else if((fabs(p3Point.z()) > fabs(p3Point.y())) && (fabs(p3Point.z()) > fabs(p3Point.x())))
		nCoord = 2;
		
	return (p3Point.e[nCoord] > 0) ? n[nCoord] : -1 * n[nCoord];
};

bool solveQuadratic(float dA, float dB, float dC, float &dT0, float &dT1)
{
	float dDiscr = dB * dB - 4.f * dA * dC;
	
	if (dDiscr <= 0.f) // Sin soluciones
		return false;
		
	float dRDiscr 	= sqrtf(dDiscr);
	float dI2A		= 1.f / (2.f * dA); 
	
	// Calculamos las raices
	dT0 = (-dB - dRDiscr) * dI2A;
	dT1 = (-dB + dRDiscr) * dI2A;
	
	// Intercambiamos si es necesario
	if(dT0 > dT1)
		std::swap(dT0, dT1);
	
	return true;
}

bool testRaySphere(
	const Ray 	&r,
	const float min,
	const float max,
	float 		&dTVal)
{
	Vec3 v3Origin;
	
	v3Origin.set(r.origin());
	
	// Se asume que la esfera tiene su centro en el origen (0,0,0) y que
	// su radio es 1.
	float a = dot(r.direction(), r.direction());
	float b = 2.f * dot(r.direction(), v3Origin);
	
	// En el espacio de la esfera, el radio es 1 siempre.
	float c = dot(v3Origin, v3Origin) - 1.f; 
	 
	float dT0, dT1;
	
	if(!solveQuadratic(a, b, c, dT0, dT1))
		return false;
	
	if(dT0 < min)
		dTVal = dT1;
	else
		dTVal = dT0;
		
	if((dTVal < min) || (dTVal > max))
		return false;
		
	return true;
}

bool testRayTriangle(
	const Ray &r,	 			// Ray to test
	const Point &p0, 			// Triangle vertexs
	const Point &p1, 
	const Point &p2, 
	float dMin, 				// Max and Min distance
	float dMax, 
	float &dTval)				// Dist to hit, if occurs
{
	float A = p0.x() - p1.x();
	float B = p0.y() - p1.y();
	float C = p0.z() - p1.z();

	float D = p0.x() - p2.x();
	float E = p0.y() - p2.y();
	float F = p0.z() - p2.z();

	float G = r.direction().x();
	float H = r.direction().y();
	float I = r.direction().z();

	float J = p0.x() - r.origin().x();
	float K = p0.y() - r.origin().y();
	float L = p0.z() - r.origin().z();
	
	float EIHF = E*I-H*F;
	float GFDI = G*F-D*I;
	float DHEG = D*H-E*G;
	
	float denom = (A*EIHF + B*GFDI + C*DHEG);

	float beta = (J*EIHF + K*GFDI + L*DHEG) / denom;
	
	if (beta <= 0.0f || beta >= 1.0f) return false;

	float AKJB = A*K - J*B;
	float JCAL = J*C - A*L;
	float BLKC = B*L - K*C;
	
	float gamma = (I*AKJB + H*JCAL + G*BLKC) / denom;
	if(gamma <= 0.0f || beta+gamma >= 1.0f) return false;
		
	float dTemp = -(F*AKJB + E*JCAL + D*BLKC) / denom;
	if (dTemp >= dMin && dTemp <= dMax) {
		dTval = dTemp;
		return true;
	}

	return false;
}

bool testRayCylinder(
	const Ray 	&r,
	const float min,
	const float max,
	float 		&dTVal)
{
	// Se asume que el cilindro está situado a lo largo del eje Z,
	// y que sus extremos son <0, 0, -1> y <0, 0, 1>
	
	float a = r.direction().x() * r.direction().x() + r.direction().y() * r.direction().y();
	float b = 2 * (r.direction().x() * r.origin().x() + r.direction().y() * r.origin().y());
	
	// Radio del cilindro = 1
	float c = r.origin().x() * r.origin().x() + r.origin().y() * r.origin().y() - 1.f;
 
	float dT0, dT1;
	
	if(!solveQuadratic(a, b, c, dT0, dT1))
		return false;
	
	if(dT0 < min) {
		// El primer punto de interseccion está tras la cámara
		dTVal = dT1;
		
		// Comprobamos el segundo.
		if((dTVal < min) || (dTVal > max))
			return false;
		
		// Ahora comprobaremos si está dentro del cilindro o fuera
		Point p3Check(Ray(r).getPoint(dTVal));
		
		if((p3Check.z() < -1.f) || (p3Check.z() > 1.f))
			return false;
	}
	else {
		// Tenemos los dos puntos por delante de la camara, habrá
		// que comprobarlos los dos.
		dTVal = dT0;
		
		if(dTVal > max)
			return false;
		
		Point p3Check(Ray(r).getPoint(dTVal));	
		
		// Si dT0 no está dentro de los márgenes, puede que dT1 sí.
		if((p3Check.z() < -1.f) || (p3Check.z() > 1.f)) {
			dTVal = dT1;
			
			if(dTVal > max)
				return false;
			
			p3Check = Ray(r).getPoint(dTVal);
			
			if((p3Check.z() < -1.f) || (p3Check.z() > 1.f))
				return false;
		}
	}
	
	return true;	
}
