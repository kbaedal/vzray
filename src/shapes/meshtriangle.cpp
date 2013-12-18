#include <cstdio>

#include "isecaux.h"
#include "mesh.h"
#include "meshtriangle.h"

MeshTriangle::MeshTriangle(const MeshFace &mfInfo, Mesh *a_pMesh)
{
	for(int i = 0; i < 3; i++) {
		nVList[i] = mfInfo.nVIndex[i];
		nNList[i] = mfInfo.nNIndex[i];
		nTList[i] = mfInfo.nTIndex[i];
	}
	
	pMesh = a_pMesh;
}

bool MeshTriangle::hit(const Ray &a_rRay, float a_dMin, float a_dMax, HitRecord &a_hrHitRcd) const
{
	float dTval;
	
	Point 	p0, p1, p2;
	Vec3	v3Dir10, v3Dir20;
	
	p0 = pMesh->m_p3VertexArray[nVList[0]];
	p1 = pMesh->m_p3VertexArray[nVList[1]];
	p2 = pMesh->m_p3VertexArray[nVList[2]];
	
	if(testRayTriangle(a_rRay, p0, p1, p2, a_dMin, a_dMax, dTval))	{
		a_hrHitRcd.dDist = dTval;
		
		if((nNList[0] > -1) && (nNList[1] > -1) && (nNList[2] > -1)) {
			Vec3 n0, n1, n2;
			
			n0 = pMesh->m_v3NormalArray[nNList[0]];
			n1 = pMesh->m_v3NormalArray[nNList[1]];
			n2 = pMesh->m_v3NormalArray[nNList[2]];
			
			a_hrHitRcd.v3Normal = versor(n0 + n1 + n2);
		}
		else {
			// No normal for each vertex, interpolate points
			v3Dir10.set(p1 - p0);
			v3Dir20.set(p2 - p0);
			
			a_hrHitRcd.v3Normal = versor(cross(v3Dir10, v3Dir20));
		}
		
		if((nTList[0] > -1) && (nTList[1] > -1) && (nTList[2] > -1)) {
			// Not implemented yet
		}
		else {
			// No texture given, use mat.
			a_hrHitRcd.pMat	= pMesh->m_pMat;
		}
		
		return true;
	}
	
	return false;
}

bool MeshTriangle::shadowHit(const Ray &a_rRay, float a_dMin, float a_dMax) const
{
	float dTval;
	
	Point p0, p1, p2;
	
	p0 = pMesh->m_p3VertexArray[nVList[0]];
	p1 = pMesh->m_p3VertexArray[nVList[1]];
	p2 = pMesh->m_p3VertexArray[nVList[2]];
	
	return testRayTriangle(a_rRay, p0, p1, p2, a_dMin, a_dMax, dTval);
}
