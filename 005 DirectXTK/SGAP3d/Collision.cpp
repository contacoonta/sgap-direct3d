
#include "stdafx.h"
#include "DXUT.h"
#include <DirectXMath.h>
#include "MeshObj.h"
#include "Collision.h"

using namespace DirectX;


/*
	충돌 체크
	Source : Ray
	Target : Mesh Triangle
*/
bool Collision::pickHit(_In_ Mesh* pmesh, _In_ XMVECTOR raypos, _In_ XMVECTOR raydir, _Out_ FLOAT& fdist, _Out_ XMVECTOR& hitpos)
{
	if (pmesh == nullptr)
		return false;

	MeshObj* pobj = dynamic_cast<MeshObj*>(pmesh);
	if (pobj == nullptr)
		return false;
	
	DWORD dwNumFaces = (DWORD)pobj->m_indexArray.size() / 3;
	FLOAT fBary1, fBary2;
	for (DWORD i = 0; i < dwNumFaces; i++)
	{
		//Temporary 3d floats for each vertex
		XMFLOAT3 v0, v1, v2;

		//Get triangle 
		v0 = pobj->m_vertexArray[pobj->m_indexArray[(i * 3) + 0]];
		v1 = pobj->m_vertexArray[pobj->m_indexArray[(i * 3) + 1]];
		v2 = pobj->m_vertexArray[pobj->m_indexArray[(i * 3) + 2]];

		XMVECTOR xv0;
		xv0 = XMLoadFloat3(&v0);
		XMVECTOR xv1;
		xv1 = XMLoadFloat3(&v1);
		XMVECTOR xv2;
		xv2 = XMLoadFloat3(&v2);

		// Check if the pick ray passes through this point
		if (IntersectTriangle(raypos, raydir, xv0, xv1, xv2, fdist, fBary1, fBary2))
		{
			hitpos = (xv0 + xv1 + xv2) / 3 ;
			return true;
		}
		else
		{
			hitpos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			fdist = 0.0f;
		}
	}

	return false;
}




//--------------------------------------------------------------------------------------
// Given a ray origin (orig) and direction (dir), and three vertices of a triangle, this
// function returns TRUE and the interpolated texture coordinates if the ray intersects 
// the triangle
//--------------------------------------------------------------------------------------
bool Collision::IntersectTriangle(const XMVECTOR& orig, const XMVECTOR& dir,
						XMVECTOR& v0, XMVECTOR& v1, XMVECTOR& v2,
                        FLOAT& t, FLOAT& u, FLOAT& v )
{
    // Find vectors for two edges sharing vert0
	XMVECTOR edge1 = v1 - v0;
	XMVECTOR edge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate U parameter
	XMVECTOR pvec;
	pvec = XMVector3Cross(dir, edge2);
    
    // If determinant is near zero, ray lies in plane of triangle
	XMVECTOR vdet = XMVector3Dot(edge1, pvec);

    XMVECTOR tvec;
	if (vdet.m128_f32[0] > 0)
    {
        tvec = orig - v0;
    }
    else
    {
        tvec = v0 - orig;
		vdet = -vdet;
    }

	if (vdet.m128_f32[0] < 0.0001f)
        return FALSE;

    // Calculate U parameter and test bounds
	XMVECTOR vu = XMVector3Dot(tvec, pvec);
	u = vu.m128_f32[0];
	
    
	if (u < 0.0f || u > vdet.m128_f32[0])	
        return FALSE;

    // Prepare to test V parameter
    XMVECTOR qvec;
	qvec = XMVector3Cross( tvec, edge1);

    // Calculate V parameter and test bounds
	XMVECTOR vv = XMVector3Dot(dir, qvec);
	v = vv.m128_f32[0];
	
	if (v < 0.0f || u + v > vdet.m128_f32[0])
        return FALSE;

    // Calculate t, scale parameters, ray intersects triangle
	XMVECTOR vt = XMVector3Dot(edge2, qvec);
	t = vt.m128_f32[0];
	
	FLOAT fInvDet = 1.0f / vdet.m128_f32[0];
    t *= fInvDet;
    u *= fInvDet;
    v *= fInvDet;

    return TRUE;
}