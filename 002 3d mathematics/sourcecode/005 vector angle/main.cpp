#pragma comment( lib, "d3d11.lib")

#include <iostream>
#include <string>

#include <d3d11.h>
#include <DirectXMath.h>

using std::wstring;
using namespace DirectX;

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
	/*
		�� ���� ������ ���� ���ϱ� ( rad )

		���� ] 
			cos(Th) = v1 dot v2	-> Th �� Radian
		acos * cos(Th) : �纯�� acos �� ���Ѵ�.
		->	Th		= acos( V1 dot V2 )
		->	Th radian -> degree
	*/

	XMFLOAT3 vf1 = { -1.0f, 1.0f, 0.0f };
	XMFLOAT3 vf2 = { 2.0f, 0.5f, 0.0f };

	XMVECTOR v1 = XMLoadFloat3(&vf1);
	XMVECTOR v2 = XMLoadFloat3(&vf2);
	
	XMVECTOR vn1 = XMVector3Normalize(v1);
	XMVECTOR vn2 = XMVector3Normalize(v2);


	// �� ������ ������ cos(Th)
	XMVECTOR vdot = XMVector3Dot(vn1, vn2);
	// 
	float rad = acosf(XMVectorGetX(vdot));
	show(L"vdot = ", vdot);
	
	float deg = XMConvertToDegrees(rad);
	wprintf(L"deg = %f\n", deg);


	// �κ����� ���� ���Ҷ� Ȱ��
	XMVECTOR vang = XMVector3AngleBetweenVectors(v1, v2);
	show(L"vangle = ", vang);

	deg = XMConvertToDegrees(XMVectorGetX(vang));
	wprintf(L"deg = %f\n", deg);
}