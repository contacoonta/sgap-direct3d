#pragma comment( lib, "d3d11.lib")

#include <iostream>
#include <string>

#include <d3d11.h>
#include <DirectXMath.h>

using std::string;
using namespace DirectX;

using std::cout;
using std::endl;

void show(string str, XMFLOAT4X4& m);
void show(string str, XMMATRIX& m);

void main()
{
	/*XMFLOAT3 vf;
	XMVECTOR v;*/

	// 저장용
	XMFLOAT4X4	fm = {	1, 2, 3, 4,
						0, 2, 0, 5,
						0, 0, 1, 0,
						0, 0, 0, 1, };	//Scale 1,2,1
	// 연산용
	//XMMATRIX	xm;						
	show("fm = ", fm);

	// 1. 단위 행렬 ( Identity Matrix )
	XMMATRIX xm = XMMatrixIdentity();
	show("Identity Matrix", xm);

	// 2. 전치 행렬 ( Transpose Matrix )
	xm = XMLoadFloat4x4(&fm);
	show("pre transpose", xm);
	xm = XMMatrixTranspose(xm);
	show("after transpose", xm);


	/*
		Scale * Rotation * Translation = World Matrix
	*/

	// 3. 크기 행렬 ( Scale )
	XMMATRIX xms = XMMatrixScaling(2.0f, 3.0f, 1.0f);
	//XMMatrixScalingFromVector(XMVECTOR);
	show("Scaling", xms);

	// 4. 이동 행렬 ( Translate )
	// 10, 7, -2
	// Offset : 현재위치 기준으로 얼마만큼 이동할지
	XMMATRIX xmt = XMMatrixTranslation(10, 7, -2);
	show("Translation", xmt);

	// 5. Scale , Translate 를 하나의 행렬로 만들기
	// Scale * Translate = World Matrix
	XMMATRIX xmw = xms * xmt;
	show("WorldMatrix", xmw);

}

void show(string str, XMFLOAT4X4& m)
{
	cout << "-------------------" << str << "-------------------" << endl;
	cout << m._11 << " , " << m._12 << " , " << m._13 << " , " << m._14 << endl		// right
		 << m._21 << " , " << m._22 << " , " << m._23 << " , " << m._24 << endl		// up
		 << m._31 << " , " << m._32 << " , " << m._33 << " , " << m._34 << endl		// forward
		 << m._41 << " , " << m._42 << " , " << m._43 << " , " << m._44 << endl;	// position
}

void show(string str, XMMATRIX& m)
{
	XMFLOAT4X4 fm;
	XMStoreFloat4x4(&fm, m);

	cout << "-------------------" << str << "-------------------" << endl;
	cout << fm._11 << " , " << fm._12 << " , " << fm._13 << " , " << fm._14 << endl		// right
		 << fm._21 << " , " << fm._22 << " , " << fm._23 << " , " << fm._24 << endl		// up
		 << fm._31 << " , " << fm._32 << " , " << fm._33 << " , " << fm._34 << endl		// forward
		 << fm._41 << " , " << fm._42 << " , " << fm._43 << " , " << fm._44 << endl;	// position
}