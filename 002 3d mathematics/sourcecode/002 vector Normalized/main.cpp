#pragma comment( lib, "d3d11.lib")

#include <iostream>

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

void main()
{
	XMFLOAT3 v3 = { 5, -5, 5 };

	//XMVECTOR �� ��ȯ
	XMVECTOR xv = XMLoadFloat3(&v3);

	wprintf(L"xmvector = %f, %f, %f\n",
						XMVectorGetX(xv),
						XMVectorGetY(xv),
						XMVectorGetZ(xv));

	// �������� ȭ
	XMVECTOR xn = XMVector3Normalize(xv);

	wprintf(L"xmvector = %f, %f, %f\n",
						XMVectorGetX(xn),
						XMVectorGetY(xn),
						XMVectorGetZ(xn));


	// �������Ϳ� Ư�� ũ�⸦ ���ϱ�
	// Vector * Scalar
	// xn * 100.0f;
	XMVECTOR xm = xn * 20.0f;

	wprintf(L"xmvector = %f, %f, %f\n",
						XMVectorGetX(xm),
						XMVectorGetY(xm),
						XMVectorGetZ(xm));


	// xm �� ũ�� -> ������ ���̰� �� ũ��
	XMVECTOR xl = XMVector3Length(xm);

	wprintf(L"xmvector = %f, %f, %f\n",
						XMVectorGetX(xl),
						XMVectorGetY(xl),
						XMVectorGetZ(xl));

	// xm �� Reverse(-)
	XMVECTOR xr = -xm;

	wprintf(L"xmvector = %f, %f, %f\n",
						XMVectorGetX(xr),
						XMVectorGetY(xr),
						XMVectorGetZ(xr));
}