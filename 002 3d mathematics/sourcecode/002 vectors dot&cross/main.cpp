#include <iostream>

#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#include <xnamath.h>

void main()
{
	/*
		1. 두 벡터의 내적 ( dot product )
	*/
	XMVECTOR v1 = { 0.0f, 1.0f, 0.0f };
	XMVECTOR v2 = { 1.0f, 0.2f, 0.0f };
	
	//벡터와 벡터 
	XMVECTOR v3 = XMVector3Dot(v1, v2);
	
	/*
		== 0 직교한다.
		< 0 뒤에 있다 or 반대 방향을 보고 있다.
		> 0 앞에 있다 or 같은 방향을 보고 있다.
	*/
	wprintf(L"dot product = %.2f, %.2f, %.2f \n", XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3));


	/*
		2. 두 벡터의 외적 ( cross product )
		- 왼손 좌표계 기준으로 foward , side , up 순으로 연산
	*/
	XMVECTOR Vfwd	= XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR Vside	= XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR Vup = XMVector3Cross(Vfwd, Vside);

	/*
		두 벡터의 up 벡터 ( normal 벡터 ) 를 구할 수 있다.
	*/
	wprintf(L"cross product = %.2f, %.2f, %.2f \n", XMVectorGetX(Vup), XMVectorGetY(Vup), XMVectorGetZ(Vup));
	
}

