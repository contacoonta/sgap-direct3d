#pragma comment( lib, "d3d11.lib")

#include <iostream>
#include <string>

#include <d3d11.h>
#include <DirectXMath.h>

using std::string;
using namespace DirectX;

using std::cout;
using std::endl;

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
void show(string str, XMFLOAT4X4& m)
{
	cout << "-------------------" << str << "-------------------" << endl;
	cout << m._11 << " , " << m._12 << " , " << m._13 << " , " << m._14 << endl		// right
		 << m._21 << " , " << m._22 << " , " << m._23 << " , " << m._24 << endl		// up
		 << m._31 << " , " << m._32 << " , " << m._33 << " , " << m._34 << endl		// forward
		 << m._41 << " , " << m._42 << " , " << m._43 << " , " << m._44 << endl;	// position
}

void main()
{
	XMFLOAT4X4 fm;
		
	XMMATRIX xm = XMLoadFloat4x4(&fm);
	xm = XMMatrixIdentity();
	//show("init matrix", xm);

	XMStoreFloat4x4(&fm, xm);
	//show("init matrix", fm);

	/*
		회전행렬 (Rotation Matrix)
	*/

	// X축 3 , Z축 5
	xm = XMMatrixRotationX(3.0f);
	show("rotaion x", xm);

	xm = XMMatrixRotationZ(0.5f);
	show("rotaion z", xm);

	xm = XMMatrixRotationY(2.0f);
	show("rotaion y", xm);

	xm = XMMatrixRotationRollPitchYaw(2.0f, 0.0f, 0.0f);
	show("rotaion Pitch", xm);

	xm = XMMatrixRotationRollPitchYaw(0.0f, 2.0f, 0.0f);
	show("rotaion Yaw", xm);

	xm = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 2.0f);
	show("rotaion Roll", xm);

	// 마지막 회전행렬에서 Forward vector 가져오기
	// 3.0f 만큼 전진하기
	

	/*
		최종행렬만들기 ( S R T -> W)
		
		S T R -> W 바꾸면 어떻게 되나 ?
	*/

	/*
		4x4 MATRIX
		1 0 0 0	Right Vector
		0 1 0 0	Up Vector
		0 0 1 0	Forward Vector
		0 0 0 1	Position
	*/


}