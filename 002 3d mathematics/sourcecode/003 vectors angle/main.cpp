#include <iostream>

#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#include <xnamath.h>

void main()
{
	/*
		1. 두 벡터 사이의 각도 구하기 ( rad -> deg )

		공식 ]	cos(Th) = v1 dot v2		->	Th 는 Radian

				Th = acos( v1 dot v2 )	->	cos 의 역 acos 을 양변에 곱한다.
	*/

	XMVECTOR v1 = { 1.0f, 0.0f, 0.0f };
	XMVECTOR v2 = { 0.0f, 1.0f, 0.0f };
	
	XMVECTOR vdot = XMVector3Dot(v1, v2);
	float rad = acosf(XMVectorGetX(vdot));
	float deg = XMConvertToDegrees(rad);

	wprintf(L"fdot = %.4f, rad = %.4f, deg = %.0f \n", XMVectorGetX(vdot), rad, deg);

	wprintf(L"----------------------------------\n");

	/*
		2. XMVector3AngleBetweenVectors 이용하기 
	*/
	//벡터와 벡터 
	XMVECTOR v3 = XMVector3AngleBetweenVectors(v1, v2);
	
	wprintf(L"angle rad = %.4f, %.4f, %.4f \n", XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3));
	wprintf(L"angle deg = %.2f \n", XMConvertToDegrees(XMVectorGetX(v3)));
}

