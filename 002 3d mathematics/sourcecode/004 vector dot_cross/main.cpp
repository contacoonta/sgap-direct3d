#pragma comment( lib, "d3d11.lib")

#include <time.h>
#include <iostream>
#include <string>

#include <d3d11.h>
#include <DirectXMath.h>

using std::wstring;
using namespace DirectX;

float randF(int nrange = 11)
{
	int nrnd = rand() % nrange;
	int nsign = rand() % 2;

	if (nsign == 0)
		nrnd = -nrnd;

	return (float)nrnd;
}

void show(wstring wstr, XMVECTOR v)
{
	wprintf(L"%s = %f, %f, %f\n",
		wstr.c_str(),
		XMVectorGetX(v),
		XMVectorGetY(v),
		XMVectorGetZ(v));
}

void main()
{
	srand(time(nullptr));

	/*
		1. �� ������ ���� ( Dot Product )
	*/
	XMFLOAT3 fv1 = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 fv2 = { 1.0f, -0.8f, 0.0f };

	//XMVECTOR�� ��ȯ
	XMVECTOR v1 = XMLoadFloat3(&fv1);
	XMVECTOR v2 = XMLoadFloat3(&fv2);

	/*
		���� ]
		== 0	�����Ѵ�. 
		> 0		�տ��ִ�. or ���� ������ ����.
		< 0		�ڿ��ִ�. or �ݴ� ������ ����. ( �����Ѵ�. )
	*/

	//���� ����
	XMVECTOR vdot = XMVector3Dot(v1, v2);
	show(L"dot product = ", vdot);


	/*
		�������� ] 
		���� ��ġ : 0, 0
		���� 5���� ����, 
		��ġ ���� x( min -10 , max 10 ) y( min -10 , max 10 )
	*/
	XMFLOAT2 enemies[5] = { { randF(), randF() },
							{ randF(), randF() },
							{ randF(), randF() },
							{ randF(), randF() },
							{ randF(), randF() } };

	XMFLOAT2 vplayer = { 0, 1 };
	XMVECTOR vp = XMLoadFloat2(&vplayer);
	
	for (auto e : enemies)
	{
		XMVECTOR ve = XMLoadFloat2(&e);

		XMVECTOR vd = XMVector2Dot(vp, ve);
		FLOAT fdot = XMVectorGetX(vd);
		if ( fdot > 0)
		{
			wprintf(L"dot = %f , Forward.\n", fdot);
		}
		else if (fdot < 0)
		{
			wprintf(L"dot = %f , Backward.\n", fdot);
		}
		else
		{
			wprintf(L"dot = %f , side.\n", fdot);
		}
	}

	
	/*
		2. �� ������ ���� ( Cross Product )
		- �޼� ��ǥ�� ����, forward, side, up ������ ����

		- �� ���Ϳ� �������� ( normal ���� , up ���� ) �� ���� �� �ִ�.
	*/

	XMVECTOR vfwd = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR vright = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);

	//XMVECTOR vup = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR vup = XMVector3Cross(vfwd, vright);
	show(L"Cross = ", vup);
}

