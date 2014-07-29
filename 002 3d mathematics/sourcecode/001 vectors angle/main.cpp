#include <iostream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

void main()
{
	/*
		1. 두 벡터 사이의 각도 구하기 ( rad -> deg )
	*/
	WCHAR str[128] = {};
	D3DXVECTOR2 v1 = D3DXVECTOR2(1.0f, 0.0f);
	D3DXVECTOR2 v2 = D3DXVECTOR2(0.0f, 1.0f);
	D3DXVec2Normalize(&v1, &v1);
	D3DXVec2Normalize(&v2, &v2);
	float fdot = D3DXVec2Dot(&v1, &v2);
	float rad = acosf(fdot);
	float deg = D3DXToDegree(rad);

	//swprintf(str, L"fdot = %.4f, rad = %.4f, deg = %.0f", fdot, rad, deg);
	wprintf(L"fdot = %.4f, rad = %.4f, deg = %.0f \n", fdot, rad, deg);
}

