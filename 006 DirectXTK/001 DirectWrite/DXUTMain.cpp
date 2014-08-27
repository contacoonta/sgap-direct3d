
#include "DXUT.h"

#pragma warning( disable : 4100 )
using namespace DirectX;



bool CALLBACK IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
									DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext)
{
	return true;
}


HRESULT CALLBACK OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
	HRESULT hr;


	return S_OK;
}


HRESULT CALLBACK OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
									const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
	HRESULT hr;


	return S_OK;
}


void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime,
	float fElapsedTime, void* pUserContext)
{
	pd3dImmediateContext->ClearRenderTargetView(DXUTGetD3D11RenderTargetView(), Colors::MidnightBlue);
	pd3dImmediateContext->ClearDepthStencilView(DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0, 0);


}


void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext)
{

}

void CALLBACK OnD3D11DestroyDevice(void* pUserContext)
{

}

bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext)
{
	return true;
}

void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{

}

LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext)
{

	return 0;
}


void CALLBACK OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
{
}



int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackKeyboard( OnKeyboard );
    DXUTSetCallbackFrameMove( OnFrameMove );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );

    DXUTSetCallbackD3D11DeviceAcceptable( IsD3D11DeviceAcceptable );
    DXUTSetCallbackD3D11DeviceCreated( OnD3D11CreateDevice );
    DXUTSetCallbackD3D11SwapChainResized( OnD3D11ResizedSwapChain );
    DXUTSetCallbackD3D11SwapChainReleasing( OnD3D11ReleasingSwapChain );
    DXUTSetCallbackD3D11DeviceDestroyed( OnD3D11DestroyDevice );
    DXUTSetCallbackD3D11FrameRender( OnD3D11FrameRender );

    DXUTInit( true, true, nullptr );
    DXUTSetCursorSettings( true, true );
    DXUTCreateWindow( L"001 DxUTK - DirectWrite" );
    DXUTCreateDevice( D3D_FEATURE_LEVEL_10_0, true, 800, 600 );

    DXUTMainLoop();

    return DXUTGetExitCode();
}

