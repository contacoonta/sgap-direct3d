#pragma comment(lib, "d3d11.lib")

#include <iostream>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

#include "Airplane.h"

void main()
{
	/*
		Vector : 크기 , 방향
	*/
	
	// x, y, z, w
	// 데이터 저장용
	XMFLOAT4 vf4 = XMFLOAT4(0,0,0,0);

	FLOAT fx = vf4.x;
	FLOAT fy = vf4.y;
	FLOAT fz = vf4.z;
	FLOAT fw = vf4.w;
	
	// 32bit x 4 = 128bit
	// x, y, z, w
	// 데이터 연산용 , 128bit
	XMVECTOR vxm = XMVectorZero();
	fx = XMVectorGetX(vxm);
	fy = XMVectorGetY(vxm);
	fz = XMVectorGetZ(vxm);
	fw = XMVectorGetW(vxm);

	vxm = XMVectorSet(1, 1, 1, 1);
	
	// XMFLOAT4 -> XMVECTOR 변환
	XMVECTOR vnew = XMLoadFloat4(&vf4);

	// XMVECTOR -> XMFLOAT4 변환
	XMStoreFloat4(&vf4, vnew);


	/*
		( 5, 100, -4 )
		Airplane 클래스 만들고,
		XMFLOAT3 멤버변수로 넣고,
		XMVECTOR 변환 출력

		Airplane 클래스
		{
		Setposition(XMFLOAT3)
		SetpositionXM(XMVECTOR)
		Show()
		}
	*/

	/*
		부모 클래스 소멸자에 virtual 을 붙이는 이유
	*/
	//Airplane ap;
	Transform * pt = new Airplane;
	//Airplane* pap = (Airplane*)(pt);
	Airplane* pap = dynamic_cast<Airplane*>(pt);

	XMVECTOR appos = XMVectorSet(1, 2, 3, 4);
	pap->setPositionXM(appos);
	pap->Show();

	delete pt;
	pt = nullptr;
}