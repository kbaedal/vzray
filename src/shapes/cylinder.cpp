#include <cmath>

#include "isecaux.h"
#include "cylinder.h"

#include "onb.h"
#include "randomc/randomc.h"

const float PI = 3.14159265358979323846;

Cylinder::Cylinder(Point a_p3P0, Point a_p3P1, float a_dRadius, Material *a_pMat)
{
		std::clog << "Creando cilindro..." << std::endl;
		// Nuestro cilindro estará definido por dos puntos y un radio,
		// siendo los dos puntos el centro de cada una de sus caras.
		// En su espacio, estos puntos serán:
		// p0 = <0, 0, -1>
		// p1 = <0, 0,  1>
		// Y su radio será siempre 1.
		// Aplicaremos las transformaciones necesarias para obtener el
		// cilindro indicado en el espacio real.
		
		// Creamos la matriz de transformación
		m_pTrans = new Transform;
				
		// Orden de operaciones para la transformación desde el cilindro
		// base al cilindro indicado:
		// 	- Trasladar nuestro cilindro 1 unidad en direccion z, así lo
		//		tendremos con su base en el origen del mundo real.
		//	- Escalarlo en x e y el valor indicado por a_dRadius, y en z
		//		primero lo escalamos a la mitad (tenemos un cilindro de
		//		dos unidades de longitud en z, lo necesitamos de 1 unidad)
		//		y después el valor resultante de la diferencia del punto 
		//		incial con el punto final (a_v3P1 - a_v3P0).
		//	- Como ya tenemos la base en el origen, solo nos queda girar
		//		el cilindro el ángulo correcto para que al trasladarlo
		//		hasta el punto inicial indicado, el punto final quede
		//		en la posicion correcta. Para ello:
		//			1. Calcularemos el vector que une los extremos del
		//				cilindro (a_v3P1 - a_v3P0) y lo normalizamos.
		//			2. Calculamos el eje de giro, producto vectorial del
		//				anterior vector con el vector z.
		//			3. Ahora solo queda calcular el angulo de giro. Para 
		//				ello utilizaremos el producto escalar, que nos
		//				dará el coseno del ángulo formado por los dos
		//				vectores.
		//	- Ahora solo queda trasladar el cilindro hasta a_v3P0.
		
		/*
		 * 
		 * PRIMERO LOS ESCALES Y ROTATES Y POR ULTIMO EL TRANSLATE
		 * 
		 */
		//~ // Trasladamos nuestro cilindro que tiene por puntos base el 
		//~ // (-1, -1, -1) y (1, 1, 1) para que el (-1, -1, -1) coincida
		//~ // con el origen.
		//~ m_pTrans->translate(0.f, 0.f, 1.f);
		//~ // Lo escalamos a la mitad en z, con lo que tenemos un cilindro
		//~ // de una unidad de largo en ese eje.
		//~ m_pTrans->scale(1.f, 1.f, .5);
		//~ 
		//~ // Ahora lo escalamos para que tenga el tamaño indicado, que 
		//~ // será la distancia entre los puntos que lo definen, y el radio 
		//~ // adecuado.
		//~ Vec3 v3Dist;
		//~ v3Dist.set(a_p3P1 - a_p3P0);
		//~ m_pTrans->scale(a_dRadius, a_dRadius, v3Dist.length());
		//~ 
		//~ std::clog << "Matrix: " << std::endl << m_pTrans->m_mtxM << std::endl;
		//~ 
		//~ std::clog << "Aplicado scale: <" << a_dRadius << ", " << a_dRadius << ", " << v3Dist.length() << ">" << std::endl;
		//~ 
		//~ Vec3 v3CilAxis, v3RotateAxis;
		//~ 
		//~ v3CilAxis.set(a_p3P1 - a_p3P0);
		//~ v3CilAxis.normalize();
		//~ 
		//~ std::clog << "Eje del cilindro: " << v3CilAxis << std::endl;
		//~ 
		//~ v3RotateAxis = cross(v3CilAxis, Vec3(0.f, 0.f, 1.f));
		//~ v3RotateAxis.normalize();
		//~ 
		//~ std::clog << "Eje de giro: " << v3RotateAxis << std::endl;
		//~ 
		//~ // Ambos son vectores normalizados, con lo que nos ahorramos
		//~ // la división por la multiplicacion de los modulos.
		//~ float dCosAng = dot(v3CilAxis, Vec3(0.f, 0.f, 1.f));
		//~ // El angulo esta en radianes, y lo necesitamos en grados sexagesimales.
		//~ float dAngRad 	= acos(dCosAng);
		//~ float dAng 		= dAngRad * (180.f / PI);
		//~ 
		//~ std::clog << "(Coseno, Angulo_Rad, Ang): (" << dCosAng << ", " << dAngRad << ", " << dAng << ")" << std::endl;
		//~ // Rotamos. 
		//~ m_pTrans->rotate(dAng, v3RotateAxis);
		//~ std::clog << "Matrix: " << std::endl << m_pTrans->m_mtxM << std::endl;
		
		// Escalamos el cilindro en x e y al radio indicado, y en z
		// lo escalaremos a la distancia qua hay entre p0 y p1.
		Vec3 	v3CilAxis,		// Eje del cilindro, y auxiliar para distancias
				v3RotateAxis;	// Eje de giro.
		Ray		rAux;			// Calculos auxilares.
		float	dLength,		// Longitud del cilindro en z.
				dCosAng,		// Coseno formado por los ejes del cilindro y de giro
				dAngRad,		// Angulo en radianes entre los ejes
				dAng;			// Angulo en grados sexagesiamales.
		
		v3CilAxis.set(a_p3P1 - a_p3P0);
		dLength = v3CilAxis.length();
		
		rAux.set(a_p3P0, versor(v3CilAxis));
		
		// Escalamos a la mitad en z, porque nuestro cilindro base ya mide
		// dos unidades en esa dirección.
		m_pTrans->scale(a_dRadius, a_dRadius, dLength/2.f);
		std::clog << "\nEscalado a : (" << a_dRadius << ", " << a_dRadius << ", " << dLength << ")\n";
		
		// Rotaremos el cilindro al eje que marque p1 - p0, para que al
		// trasladarlo hasta su lugar, los puntos coincidan.
		v3CilAxis.normalize();
		
		// El eje de rotación será el resultado del producto vectorial
		// del eje del cilindro con el eje z, que es donde está orientado
		// el cilindro base.
		// Comprobamos que el angulo que forman los vectores es mayor que 0,
		// es decir, que no son paralelos.
		
		std::clog << "\nEje del cilindro: " << v3CilAxis << "\n";
		
		dCosAng = dot(v3CilAxis, Vec3(0.f, 0.f, 1.f));
		std::clog << "\nCoseno: " << dCosAng << "\n";
		if(fabs(dCosAng) < 1.f)
		{
			v3RotateAxis = cross(v3CilAxis, Vec3(0.f, 0.f, 1.f));
			v3RotateAxis.normalize();
			
			std::clog << "\nEje de rotacion: " << v3RotateAxis << "\n";
			
			// Calculamos ahora el ángulo que forman el eje del cilindro
			// con el eje z. Esto nos dirá cuanto girar el cilindro
			// sobre el eje de rotación calculado.
			// Ambos son vectores normalizados, con lo que nos ahorramos
			// la división por la multiplicacion de los modulos.
			
			// El angulo esta en radianes, y lo necesitamos en grados sexagesimales.
			dAngRad 	= acos(dCosAng);
			dAng 		= dAngRad * (180.f / PI);
			
			std::clog << "Angulo de rotacion (Sexagesimales): " << dAng << "\n";
			std::clog << "Angulo de rotacion (Radianes): " << dAngRad << "\n";
			
			// Rotamos. 
			m_pTrans->rotate(dAng, v3RotateAxis);
		}
		else {
			std::clog << "Los ejes son paralelos. No es necesaria la rotacion.\n";
		}	
		
		// Trasladamos el cilindro una vez girado al punto base indicado.
		// Para ello, trasladaremos nuestro cilindro que tiene su centro
		// en el origen al centro que marcan las coordenadas p0 y p1.
		m_pTrans->translate(rAux.getPoint(dLength/2.f));
		
		std::clog << "Trasladamos a: " << rAux.getPoint(dLength/2.f) << "\n";
				
		// Iniciamos nuestra bbox
		m_abBox.set(Point(-1.f), Point(1.f));	
		
		// Y le aplicamos la transformación.		
		m_abBox = m_pTrans->updateAABB(m_abBox);
		
		m_p3P0		= Point(0.f, 0.f, -1.f);
		m_p3P1		= Point(0.f, 0.f, 1.f);
		m_dRadius	= 1.f;
		m_pMat 		= a_pMat;
		bShadow		= true;
		bBounds 	= true;
}

bool Cylinder::hit(const Ray &a_rRay, float a_dMin, float a_dMax, HitRecord &a_hrHitRcd) const
{	
	// Transformar el rayo
	Ray rObjSpace = m_pTrans->sceneToObject(a_rRay);
	
	float dTVal;
	
	if(testRayCylinder(rObjSpace, a_dMin, a_dMax, dTVal)) {
		a_hrHitRcd.dDist 	= dTVal;
		// Para la normal, tomaremos el punto de intersección, y pondremos
		// a 0.f su coordenada z (el cilindro esta centrado en el eje Z).
		Point p3PI(rObjSpace.getPoint(dTVal));
		
		a_hrHitRcd.v3Normal = versor(m_pTrans->objectNormalToScene(Vec3(p3PI.x(), p3PI.y(), 0.f)));
		
		a_hrHitRcd.pMat		= m_pMat;
		
		return true;
	}
	
	return false;
}

bool Cylinder::shadowHit(const Ray &a_rRay, float a_dMin, float a_dMax) const
{
	if(bShadow) {
		// Transformar el rayo
		Ray rObjSpace = m_pTrans->sceneToObject(a_rRay);
	
		float dTVal;
	
		return testRayCylinder(rObjSpace, a_dMin, a_dMax, dTVal);
	}
	
	return false;
}

bool Cylinder::getRandomPoint(const Point &p3ViewPoint, CRandomMersenne *rngGen, Point &p3LPoint) const
{
	return false;
}
