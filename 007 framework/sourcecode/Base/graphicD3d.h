#ifndef _GRAPHICD3D_H_
#define _GRAPHICD3D_H_

#include "basedef.h"


SGAP_BEGIN

typedef class graphicD3d
{
public:
	static bool Create(graphicD3d** ppgd3d, int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	static void Destroy(graphicD3d** ppgd3d);

public:
	graphicD3d();
	graphicD3d(const graphicD3d& d3d);
	~graphicD3d();

public:
	ID3D11Device*			GetDevice()		{ return m_device; }
	ID3D11DeviceContext*	GetDevContext()	{ return m_deviceContext; }

	XMMATRIX			GetMatrixProj()		{ return m_projectionMatrix; }
	XMMATRIX			GetMatrixWorld()	{ return m_worldMatrix; }
	XMMATRIX			GetMatrixOrtho()	{ return m_orthoMatrix; }

	UINT				GetVideoMemory()	{ return m_videoMemory; }
	
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

	XMMATRIX	 m_projectionMatrix;
	XMMATRIX	 m_worldMatrix;
	XMMATRIX	 m_orthoMatrix;

}GRAPHICD3D, *LPGRAPHICD3D;

SGAP_END

#endif