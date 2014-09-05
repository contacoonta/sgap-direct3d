
#include "stdafx.h"
#include "DXUT.h"
#include <DirectXMath.h>
#include "MeshObj.h"
#include "Collision.h"


using namespace DirectX;


float Collision::PickFromRay(_In_ Mesh* pmesh, _In_ XMVECTOR raypos, _In_ XMVECTOR raydir)
{
	MeshObj* pobj = dynamic_cast<MeshObj*>(pmesh);
	if (pobj == nullptr)
		return -1.0f;

//	raydir = XMVector3Normalize(raydir);

	XMMATRIX world = pobj->m_world;

	// 인덱스 수만큼 돌면서 , 삼각형 충돌 체크
	for (int i = 0; i < pobj->m_indexArray.size() / 3; i++)
	{
		//Triangle's vertices V1, V2, V3
		XMVECTOR tri1V1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR tri1V2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR tri1V3 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		//Temporary 3d floats for each vertex
		XMFLOAT3 tV1, tV2, tV3;

		//Get triangle 
		tV1 = pobj->m_vertexArray[pobj->m_indexArray[(i * 3) + 0]];
		tV2 = pobj->m_vertexArray[pobj->m_indexArray[(i * 3) + 1]];
		tV3 = pobj->m_vertexArray[pobj->m_indexArray[(i * 3) + 2]];

		tri1V1 = XMVectorSet(tV1.x, tV1.y, tV1.z, 0.0f);
		tri1V2 = XMVectorSet(tV2.x, tV2.y, tV2.z, 0.0f);
		tri1V3 = XMVectorSet(tV3.x, tV3.y, tV3.z, 0.0f);

		//삼각형 각 점들을 현 오브젝트의 월드 좌표로 변환
		tri1V1 = XMVector3TransformCoord(tri1V1, world);
		tri1V2 = XMVector3TransformCoord(tri1V2, world);
		tri1V3 = XMVector3TransformCoord(tri1V3, world);

		//삼각형 두 변의 외적을 이용하여 , 면의 Normal 벡터를 구함
		XMVECTOR U = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR V = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR faceNormal = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		U = tri1V2 - tri1V1;
		V = tri1V3 - tri1V1;

		faceNormal = XMVector3Cross(U, V);
		faceNormal = XMVector3Normalize(faceNormal);


		//Calculate a point on the triangle for the plane equation
		XMVECTOR triPoint = tri1V1;

		// 삼각형으로 면 만들기 공식
		// ("Ax + By + Cz + D = 0")
		float tri1A = XMVectorGetX(faceNormal);
		float tri1B = XMVectorGetY(faceNormal);
		float tri1C = XMVectorGetZ(faceNormal);
		float tri1D = (-tri1A*XMVectorGetX(triPoint) - tri1B*XMVectorGetY(triPoint) - tri1C*XMVectorGetZ(triPoint));


		//삼각형으로 만든 면과 Ray가 충돌을 하는 지점을 계산
		float ep1, ep2, t = 0.0f;
		float planeIntersectX, planeIntersectY, planeIntersectZ = 0.0f;
		XMVECTOR pointInPlane = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		ep1 = (XMVectorGetX(raypos) * tri1A) + (XMVectorGetY(raypos) * tri1B) + (XMVectorGetZ(raypos) * tri1C);
		ep2 = (XMVectorGetX(raydir) * tri1A) + (XMVectorGetY(raydir) * tri1B) + (XMVectorGetZ(raydir) * tri1C);

		//Make sure there are no divide-by-zeros
		if(ep2 != 0.0f)
			t = -(ep1 + tri1D)/(ep2);

		// t > 0 , 카메라 뒤는 pick 하지 않도록 체크
		if(t > 0.0f)
		{
			// 충돌 포인트를 정확히 계산
			planeIntersectX = XMVectorGetX(raypos) + XMVectorGetX(raydir) * t;
			planeIntersectY = XMVectorGetY(raypos) + XMVectorGetY(raydir) * t;
			planeIntersectZ = XMVectorGetZ(raypos) + XMVectorGetZ(raydir) * t;

			pointInPlane = XMVectorSet(planeIntersectX, planeIntersectY, planeIntersectZ, 0.0f);

			/*Call function to check if point is in the triangle*/
			if(PointInTriangle(tri1V1, tri1V2, tri1V3, pointInPlane))
			{
				/*Return the distance to the hit, so you can check all the other pickable objects in your scene
				and choose whichever object is closest to the camera*/
				return t/2.0f;
			}
		}
	}

	//픽킹이 실패하면 , 무한의 값을 리턴하여 의미 없게 한다.
	return FLT_MAX;
}



bool Collision::PointInTriangle(XMVECTOR& triV1, XMVECTOR& triV2, XMVECTOR& triV3, XMVECTOR& point)
{
	// 삼각형 안에서 충돌 지점 여부 판단하기
	XMVECTOR cp1 = XMVector3Cross((triV3 - triV2), (point - triV2));
	XMVECTOR cp2 = XMVector3Cross((triV3 - triV2), (triV1 - triV2));
	if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
	{
		cp1 = XMVector3Cross((triV3 - triV1), (point - triV1));
		cp2 = XMVector3Cross((triV3 - triV1), (triV2 - triV1));
		if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
		{
			cp1 = XMVector3Cross((triV2 - triV1), (point - triV1));
			cp2 = XMVector3Cross((triV2 - triV1), (triV3 - triV1));
			if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
			{
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	return false;
}