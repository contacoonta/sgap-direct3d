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
	XMFLOAT3 vf1 = { 2.0f, 0.0f, 0.0f };
	XMFLOAT3 vf2 = { 0.0f, 2.0f, 0.0f };

	XMVECTOR v1 = XMLoadFloat3(&vf1);
	XMVECTOR v2 = XMLoadFloat3(&vf2);

	//vf1 �� ũ�� , vf2 �� ũ��
	XMVECTOR vl = XMVector3Length(v1);
	show(L"V1 size", vl);
	vl = XMVector3Length(v2);
	show(L"V2 size", vl);

	XMVECTOR vadd = v1 + v2;
	vl = XMVector3Length(vadd);
	show(L"Vadd size", vl);

	// �κ��� �� , ũ��
	XMVECTOR vsub = v1 + (-v2);
	vl = XMVector3Length(vsub);
	show(L"Vsub size", vl);
	
	// ���ͳ��� �� , ���Ϳ� ��Į�� �� ( ũ�� )
	XMVECTOR vmul = XMVectorMultiply(v1, v2);
	show(L"Vmul", vmul);

	XMVECTOR vmulScl = (v1 + v2) * 5.0f;
	show(L"VmulScalar", vmulScl);


	// XMVectorSubtractAngles
	XMVECTOR subang = XMVectorSubtractAngles(v1, v2);
	show(L"SubAngle", subang);

	// XMVector3AngleBetweenNormals
	// ��������ȭ �� �� ������ ���̰� ( Radian )
	XMVECTOR nv1 = XMVector3Normalize(v1);
	XMVECTOR nv2 = XMVector3Normalize(v2);
	XMVECTOR nAng = XMVector3AngleBetweenNormals(nv1, nv2);
	show(L"NorAngle", nAng);

	FLOAT fang = XMConvertToDegrees(XMVectorGetX(nAng));
	wprintf(L"Degree Angle = %f\n", fang);


	// XMVector3AngleBetweenVectors
	// �Ϲݺ��� �ΰ��� ���̰� ( Radian )
	nAng = XMVector3AngleBetweenVectors(v1, v2);
	show(L"VectorAngle", nAng);


	// XMVectorModAngles
	// 2PI �̳��� ������ ������ ����
	XMVECTOR modang = XMVectorModAngles(nAng);
	show(L"ModAngle", modang);

}