#include <iostream>

#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#include <xnamath.h>

void main()
{
	/*
		1. �� ������ ���� ( dot product )
	*/
	XMVECTOR v1 = { 0.0f, 1.0f, 0.0f };
	XMVECTOR v2 = { 1.0f, 0.2f, 0.0f };
	
	//���Ϳ� ���� 
	XMVECTOR v3 = XMVector3Dot(v1, v2);
	
	/*
		== 0 �����Ѵ�.
		< 0 �ڿ� �ִ� or �ݴ� ������ ���� �ִ�.
		> 0 �տ� �ִ� or ���� ������ ���� �ִ�.
	*/
	wprintf(L"dot product = %.2f, %.2f, %.2f \n", XMVectorGetX(v3), XMVectorGetY(v3), XMVectorGetZ(v3));


	/*
		2. �� ������ ���� ( cross product )
		- �޼� ��ǥ�� �������� foward , side , up ������ ����
	*/
	XMVECTOR Vfwd	= XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR Vside	= XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR Vup = XMVector3Cross(Vfwd, Vside);

	/*
		�� ������ up ���� ( normal ���� ) �� ���� �� �ִ�.
	*/
	wprintf(L"cross product = %.2f, %.2f, %.2f \n", XMVectorGetX(Vup), XMVectorGetY(Vup), XMVectorGetZ(Vup));
	
}

