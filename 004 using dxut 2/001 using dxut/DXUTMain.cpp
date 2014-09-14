
#include "DXUT.h"

#pragma warning( disable : 4100 )
using namespace DirectX;


HRESULT CALLBACK OnD3D11DeviceCreated(_In_ ID3D11Device* pd3dDevice, _In_ const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, _In_opt_ void* pUserContext)
{
	// �ʱ�ȭ �ڵ� �ۼ�...

	return S_OK;
}

void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice,
				ID3D11DeviceContext* pd3dImmediateContext,
				double fTime, float fElapsedTime, void* pUserContext)
{
	pd3dImmediateContext->ClearRenderTargetView(DXUTGetD3D11RenderTargetView(), Colors::Crimson);
	pd3dImmediateContext->ClearDepthStencilView(DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	// �׸��׸� �ڵ� �ۼ�...

}

void CALLBACK OnD3D11DeviceDestroyed(void* pUserContext)
{
	// �� ���� �ڵ� �ۼ�...

}



int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
    // ����̽� ������ ������ ���ϸ�, ���
#ifdef _DEBUG
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// ������Ʈ
	DXUTSetCallbackFrameMove();
	// Ű����
	DXUTSetCallbackKeyboard();
	// �޽��� ���ν���
	DXUTSetCallbackMsgProc();


	DXUTSetCallbackD3D11DeviceCreated(OnD3D11DeviceCreated);
	// ȭ�� ũ�� ��ȯ
	DXUTSetCallbackD3D11SwapChainResized();
	DXUTSetCallbackD3D11FrameRender(OnD3D11FrameRender);
	DXUTSetCallbackD3D11DeviceDestroyed(OnD3D11DeviceDestroyed);
    
    DXUTInit( true, true, nullptr );
    DXUTSetCursorSettings( true, true );
    DXUTCreateWindow( L"001 Using DXUT" );
    DXUTCreateDevice( D3D_FEATURE_LEVEL_10_0, true, 800, 600 );
    DXUTMainLoop();
	
    return DXUTGetExitCode();
}
