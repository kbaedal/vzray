#include "test.h"

#include "matrix/matrix.h"
#include "matrix/transform.h"

bool Test::launch_test()
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

	M = T.mtx * T.inv;

	std::clog << M << "\n";

	P.set(5, 5, 5);
	std::clog << "Transformando punto: " << P << "\n";
	P = T.mtx.transform(P);
	std::clog << "Punto Transformado: " << P << "\n";
	P = T.inv.transform(P);
	std::clog << "Punto Destransformado: " << P << "\n";

	V.set(0, 0, 1);
	std::clog << "Transformando vector: " << V << "\n";
	V = T.mtx.transform(V);
	std::clog << "Vector Transformado: " << V << "\n";
	V = T.inv.transform(V);
	std::clog << "Vector Destransformado: " << V << "\n";

	R.orig = P;
	R.dir = V;
	R.refresh_inv();

	std::clog << "Transformando Rayo: " << R << "\n";
	R = T.scene_to_object(R);
	std::clog << "Rayo Transformado: " << R << "\n";
	R = T.object_to_scene(R);
	std::clog << "Punto Destransformado: " << R << "\n";

	std::clog << "Test finalizado.\n";

	return true;
}
