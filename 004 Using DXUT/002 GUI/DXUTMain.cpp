/*
	1. DXUT Ȱ�� - ���� ���

	2. GUI Ȱ�� - Button , Slider ���.. �����

	���� ]

		1. SLIDER ����� -> ��ġ�� ���ڷ� ���

		2. ä��â �����
			- CDXUTEditBox �� �̿��ؼ�
			- TEXTHELPER �Ǵ� CDXUTStatic ���� ä�� ���� ���
	�ɼ� ]
		3. ���� ��ư �����
			- ���� 3���� -> ��� �����߳� ���
*/


#include "DXUT.h"
#include "DXUTgui.h"
#include "SDKmisc.h"


#pragma warning( disable : 4100 )
using namespace DirectX;


CDXUTDialogResourceManager  g_resMgr;
CDXUTTextHelper*            g_text = nullptr;
CDXUTDialog					g_GUI;


#define GUI_BUTTON1	1
#define GUI_BUTTON2	2

#define GUI_SLIDER1 3



void CALLBACK OnGUIEvent(_In_ UINT nEvent, _In_ int nControlID, _In_ CDXUTControl* pControl,
	_In_opt_ void* pUserContext)
{
	UINT n1 = nEvent;
	int n2 = nControlID;

	switch (nControlID)
	{
	case GUI_BUTTON1:
		DXUTToggleFullScreen();
		break;

	case GUI_BUTTON2:
		DXUTShutdown();
		break;
	}	

}

void GUIInit()
{
	g_GUI.Init(&g_resMgr);
	g_GUI.SetCallback(OnGUIEvent);
	g_GUI.SetLocation(50, 10);
	g_GUI.SetSize(200, 200);

	g_GUI.AddButton(GUI_BUTTON1, L"FULLSCREEN", 50, 150, 100, 30);
	g_GUI.AddButton(GUI_BUTTON2, L"EXIT", 50, 190, 100, 30);

	g_GUI.AddSlider(GUI_SLIDER1, 50, 250, 200, 30);
}



HRESULT CALLBACK OnD3D11DeviceCreated(_In_ ID3D11Device* pd3dDevice, _In_ const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, _In_opt_ void* pUserContext)
{
	// �ʱ�ȭ �ڵ� �ۼ�...

	g_resMgr.OnD3D11CreateDevice(pd3dDevice, DXUTGetD3D11DeviceContext());
	g_text = new CDXUTTextHelper(pd3dDevice, DXUTGetD3D11DeviceContext(), &g_resMgr, 15);

	return S_OK;
}


HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, 
										  void* pUserContext )
{
	HRESULT hr;

	// ������ ũ�� ( ȭ��ũ�� ) �� ����ɶ� ȣ��Ǵ� �Լ�
	g_resMgr.OnD3D11ResizedSwapChain(pd3dDevice, pBackBufferSurfaceDesc);

    return S_OK;
}

void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice,
				ID3D11DeviceContext* pd3dImmediateContext,
				double fTime, float fElapsedTime, void* pUserContext)
{
	pd3dImmediateContext->ClearRenderTargetView(DXUTGetD3D11RenderTargetView(), Colors::Black);
	pd3dImmediateContext->ClearDepthStencilView(DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	//GUI �׸���
	g_GUI.OnRender(fElapsedTime);



	// �׸��׸� �ڵ� �ۼ�...
	g_text->Begin();

	g_text->SetInsertionPos(5, 10);
	g_text->SetForegroundColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	
	g_text->DrawTextLine(L"TEXT word");	
	g_text->DrawFormattedTextLine(L"FPS = %.2f", DXUTGetFPS());
	g_text->DrawTextLine(DXUTGetDeviceStats());

	g_text->End();
}

void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext)
{
	g_resMgr.OnD3D11ReleasingSwapChain();
}

void CALLBACK OnD3D11DeviceDestroyed(void* pUserContext)
{
	// �� ���� �ڵ� �ۼ�...
	SAFE_DELETE(g_text);
	g_resMgr.OnD3D11DestroyDevice();

}


LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{

	// Pass messages to dialog resource manager calls so GUI state is updated correctly
	g_resMgr.MsgProc(hWnd, uMsg, wParam, lParam);
	g_GUI.MsgProc(hWnd, uMsg, wParam, lParam);

    return 0;
}



int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
    // ����̽� ������ ������ ���ϸ�, ���
#ifdef _DEBUG
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// ������Ʈ
	//DXUTSetCallbackFrameMove();
	// Ű����
	//DXUTSetCallbackKeyboard();
	// �޽��� ���ν���
	DXUTSetCallbackMsgProc(MsgProc);


	DXUTSetCallbackD3D11DeviceCreated(OnD3D11DeviceCreated);
	// ȭ�� ũ�� ��ȯ, �̵� , ��ȯ ȣ��Ǵ� �Լ�
	DXUTSetCallbackD3D11SwapChainResized(OnD3D11ResizedSwapChain);
	DXUTSetCallbackD3D11FrameRender(OnD3D11FrameRender);
	// ȭ�� ũ�� ��ȯ, �̵� , ��ȯ ȣ��Ǵ� �Լ�
	DXUTSetCallbackD3D11SwapChainReleasing(OnD3D11ReleasingSwapChain);
	DXUTSetCallbackD3D11DeviceDestroyed(OnD3D11DeviceDestroyed);
    

	// UI�ʱ�ȭ �ڵ� �ۼ�
	GUIInit();

    DXUTInit( true, true, nullptr );
    DXUTSetCursorSettings( true, true );
    DXUTCreateWindow( L"002 GUI" );
    DXUTCreateDevice( D3D_FEATURE_LEVEL_10_0, true, 800, 600 );
    DXUTMainLoop();
	
    return DXUTGetExitCode();
}
