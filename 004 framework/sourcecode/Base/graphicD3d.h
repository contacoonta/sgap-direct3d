#ifndef _GRAPHICD3D_H_
#define _GRAPHICD3D_H_

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")


#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

#include "basedef.h"


SGAP_BEGIN

typedef class graphicD3d
{
public:
	static bool CreateGraphicD3d(graphicD3d** ppgd3d, int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	static void DestroyGraphicD3d(graphicD3d** ppgd3d);

public:
	graphicD3d();
	graphicD3d(const graphicD3d& d3d);
	~graphicD3d();

	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	void Release();

	void BegineScene();
	void EndScene();

}GRAPHICD3D, *LPGRAPHICD3D;

SGAP_END

#endif