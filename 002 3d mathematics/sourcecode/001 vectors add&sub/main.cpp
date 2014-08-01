#include <iostream>

#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#include <xnamath.h>


void main()
{

	/*
		1. �� ������ �� �� ��
	*/
	XMVECTOR v1 = { 1.0f, 0.0f, 0.0f };
	XMVECTOR v2 = { 0.0f, 1.0f, 0.0f };
	
	//���Ϳ� ���� 
	XMVECTOR v3 = v1 + v2;
	wprintf(L"add = %f, %f, %f \n", XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3));

	//���Ϳ� ��Į��� �ȵȴ�. ( ��, ���� Ÿ�Գ��� ���� ���� )
	XMVECTOR v4 = v1 - v2;
	wprintf(L"sub = %f, %f, %f \n", XMVectorGetX(v4), XMVectorGetY(v4), XMVectorGetZ(v4));


	/*
		2. ���Ϳ� ��Į�� �� �� ��������ȭ
	*/
	//���Ϳ� ��Į��
	FLOAT fS = 3.0f;
	v3 = v3 * fS;
	wprintf(L"multiply with Scalar = %f, %f, %f \n", XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3));

	v3 = XMVector3Normalize(v3);
	wprintf(L"normalized = %f, %f, %f \n", XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3));

	//���Ϳ� ����
	XMVECTOR v5 = { 2.0f, 1.0f, 0.0f };
	v3 = v3 * v5;
	wprintf(L"multiply with Vector = %f, %f, %f \n", XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3));

	v3 = XMVector3Normalize(v3);
	wprintf(L"normalized = %f, %f, %f \n", XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3));

}

