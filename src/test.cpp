#include "test.h"

#include "matrix/matrix.h"
#include "matrix/transform.h"

bool Test::LaunchTest()
{
	std::clog << "Lanzando Test...\n";
	
	Transform 	T;
	Matrix4x4 	M;
	Point		P;
	Vec3		V;
	Ray			R;
	
	/*
	std::clog << "Escalando T...\n";
	T.scale(1, 2, 3);
	
	
	std::clog << "Rotando Y T...\n";
	T.rotateY(56);
	*/
	std::clog << "Trasladando T...\n";
	T.translate(1, 2, 3);
	
	std::clog << "Resultado (M, I, MxI):...\n";
	std::clog << T << "\n";
	
	M = T.m_mtxM * T.m_mtxI;
	
	std::clog << M << "\n";
	
	P.set(5, 5, 5);
	std::clog << "Transformando punto: " << P << "\n";
	P = T.m_mtxM.transform(P);
	std::clog << "Punto Transformado: " << P << "\n";
	P = T.m_mtxI.transform(P);
	std::clog << "Punto Destransformado: " << P << "\n";
		
	V.set(0, 0, 1);
	std::clog << "Transformando vector: " << V << "\n";
	V = T.m_mtxM.transform(V);
	std::clog << "Vector Transformado: " << V << "\n";
	V = T.m_mtxI.transform(V);
	std::clog << "Vector Destransformado: " << V << "\n";
	
	R.m_p3Orig = P;
	R.m_v3Dir = V;
	R.refreshInv();
	
	std::clog << "Transformando Rayo: " << R << "\n";
	R = T.sceneToObject(R);
	std::clog << "Rayo Transformado: " << R << "\n";
	R = T.objectToScene(R);
	std::clog << "Punto Destransformado: " << R << "\n";
	
	std::clog << "Test finalizado.\n";	
	return true;
}
