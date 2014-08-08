#include <iostream>

#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#include <xnamath.h>

using std::cout;
using std::endl;


void show(XMMATRIX& m, char* str);

void main()
{
	XMMATRIX m;
	show(m, "matrix");

	/*
		1. identity
	*/
	XMMATRIX Mi = XMMatrixIdentity();
	show(Mi, "identity");

	/*
		2. transpose
	*/
	XMMATRIX M1 = XMMatrixSet(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	show(M1, "set");

	XMMATRIX Mtrs = XMMatrixTranspose(M1);
	show(Mtrs, "transpose");

	/*
		3. scale
	*/
	XMMATRIX Ms = XMMatrixScaling(1.0f, 2.0f, 3.0f);
	show(Ms, "scale");

	/*
		4. rotation
	*/
	XMMATRIX Mr = XMMatrixRotationX(3.0f);
	show(Mr, "rotation");

	/*
		5. translate
	*/
	XMMATRIX M2 = XMMatrixTranslation(1.0f, 3.0f, -2.0f);
	show(M2, "translate");

}

void show(XMMATRIX& m, char* str)
{
	cout << "--------------" << str << "-----------------" << endl;
	cout << m._11 << " , " << m._12 << " , " << m._13 << " , " << m._14 << endl		// right
		 << m._21 << " , " << m._22 << " , " << m._23 << " , " << m._24 << endl		// up
		 << m._31 << " , " << m._32 << " , " << m._33 << " , " << m._34 << endl		// foward
		 << m._41 << " , " << m._42 << " , " << m._43 << " , " << m._44 << endl;	// position
}