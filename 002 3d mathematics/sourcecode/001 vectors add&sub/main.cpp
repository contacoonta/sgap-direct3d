#include <iostream>

#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#include <xnamath.h>


void main()
{

	/*
		1. 크기 magnitude 
		- 벡터의 거리
	*/
	XMVECTOR v1 = {1.0f, 1.0f, 1.0f};

	XMVECTOR vR1 = XMVector3Length(v1);
	wprintf(L"mag = %f\n", XMVectorGetX(vR1));

	/*
		2. 단위 벡터 normalized vector
		- 길이가 1인 벡터
	*/
	XMVECTOR vR2 = XMVector3Normalize(v1);
	wprintf(L"unit = %f\n", XMVectorGetX(vR2));


	/*
		1. 두 벡터의 합 과 차
	*/
	XMVECTOR v1 = { 1.0f, 0.0f, 0.0f };
	XMVECTOR v2 = { 0.0f, 1.0f, 0.0f };
	
	//벡터와 벡터 
	XMVECTOR v3 = v1 + v2;
	wprintf(L"add = %f, %f, %f \n", XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3));

	//벡터와 스칼라는 안된다. ( 즉, 같은 타입끼리 연산 가능 )
	XMVECTOR v4 = v1 - v2;
	wprintf(L"sub = %f, %f, %f \n", XMVectorGetX(v4), XMVectorGetY(v4), XMVectorGetZ(v4));


	/*
		2. 벡터와 스칼라 곱 과 단위벡터화
	*/
	//벡터와 스칼라
	FLOAT fS = 3.0f;
	v3 = v3 * fS;
	wprintf(L"multiply with Scalar = %f, %f, %f \n", XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3));

	v3 = XMVector3Normalize(v3);
	wprintf(L"normalized = %f, %f, %f \n", XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3));

	//벡터와 벡터
	XMVECTOR v5 = { 2.0f, 1.0f, 0.0f };
	v3 = v3 * v5;
	wprintf(L"multiply with Vector = %f, %f, %f \n", XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3));

	v3 = XMVector3Normalize(v3);
	wprintf(L"normalized = %f, %f, %f \n", XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3));

}

