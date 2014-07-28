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
	ID3D11Device*			GetDevice()		{ return m_device; }
	ID3D11DeviceContext*	GetDevContext()	{ return m_deviceContext; }

	D3DXMATRIX			GetMatrixProj()		{ return m_projectionMatrix; }
	D3DXMATRIX			GetMatrixWorld()	{ return m_worldMatrix; }
	D3DXMATRIX			GetMatrixOrtho()	{ return m_orthoMatrix; }

	UINT				GetVideoMemory()	{ return m_videoMemory; }

public:
	graphicD3d();
	graphicD3d(const graphicD3d& d3d);
	~graphicD3d();

	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	void Release();

	void BegineScene();
	void EndScene();

private:
	UINT		m_videoMemory			= 0;
	WCHAR		m_videoDescription[128];
	BOOL		m_vsync					= false;

	D3D_DRIVER_TYPE				m_driverType		= D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL			m_featureLevel		= D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*				m_device			= NULL;
	ID3D11DeviceContext*		m_deviceContext		= NULL;
	IDXGISwapChain*				m_swapChain			= NULL;
	ID3D11RenderTargetView*		m_renderTargetView	= NULL;
	ID3D11Texture2D*			m_depthStencilbuffer= NULL;	//아직 정리 안함.
	ID3D11DepthStencilState*	m_depthStencilState	= NULL; //
	ID3D11DepthStencilView*		m_depthStencilView	= NULL; //
	ID3D11RasterizerState*		m_rasterState		= NULL; //

	D3DXMATRIX	 m_projectionMatrix;
	D3DXMATRIX	 m_worldMatrix;
	D3DXMATRIX	 m_orthoMatrix;

}GRAPHICD3D, *LPGRAPHICD3D;

SGAP_END

#endif