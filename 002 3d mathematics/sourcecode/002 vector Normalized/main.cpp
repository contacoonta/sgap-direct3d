#pragma comment( lib, "d3d11.lib")

#include <iostream>

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

void main()
{
	XMFLOAT3 v3 = { 5, -5, 5 };

	//XMVECTOR 로 변환
	XMVECTOR xv = XMLoadFloat3(&v3);

	wprintf(L"xmvector = %f, %f, %f\n",
						XMVectorGetX(xv),
						XMVectorGetY(xv),
						XMVectorGetZ(xv));

	// 단위벡터 화
	XMVECTOR xn = XMVector3Normalize(xv);

	wprintf(L"xmvector = %f, %f, %f\n",
						XMVectorGetX(xn),
						XMVectorGetY(xn),
						XMVectorGetZ(xn));


	// 단위벡터에 특정 크기를 곱하기
	// Vector * Scalar
	// xn * 100.0f;
	XMVECTOR xm = xn * 20.0f;

	wprintf(L"xmvector = %f, %f, %f\n",
						XMVectorGetX(xm),
						XMVectorGetY(xm),
						XMVectorGetZ(xm));


	// xm 의 크기 -> 벡터의 길이가 곧 크기
	XMVECTOR xl = XMVector3Length(xm);

	wprintf(L"xmvector = %f, %f, %f\n",
						XMVectorGetX(xl),
						XMVectorGetY(xl),
						XMVectorGetZ(xl));

	// xm 의 Reverse(-)
	XMVECTOR xr = -xm;

	wprintf(L"xmvector = %f, %f, %f\n",
						XMVectorGetX(xr),
						XMVectorGetY(xr),
						XMVectorGetZ(xr));
}