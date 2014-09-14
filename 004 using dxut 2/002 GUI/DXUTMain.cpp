/*
	1. DXUT 활용 - 문자 출력

	2. GUI 활용 - Button , Slider 등등.. 만들기

	과제 ]

		1. SLIDER 만들고 -> 수치를 문자로 출력

		2. 채팅창 만들기
			- CDXUTEditBox 를 이용해서
			- TEXTHELPER 또는 CDXUTStatic 으로 채팅 문자 출력
	옵션 ]
		3. 라디오 버튼 만들기
			- 보기 3개로 -> 어떤거 선택했나 출력
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
	// 초기화 코드 작성...

	g_resMgr.OnD3D11CreateDevice(pd3dDevice, DXUTGetD3D11DeviceContext());
	g_text = new CDXUTTextHelper(pd3dDevice, DXUTGetD3D11DeviceContext(), &g_resMgr, 15);

	return S_OK;
}


HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, 
										  void* pUserContext )
{
	HRESULT hr;

	// 윈도우 크기 ( 화면크기 ) 가 변경될때 호출되는 함수
	g_resMgr.OnD3D11ResizedSwapChain(pd3dDevice, pBackBufferSurfaceDesc);

    return S_OK;
}

void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice,
				ID3D11DeviceContext* pd3dImmediateContext,
				double fTime, float fElapsedTime, void* pUserContext)
{
	pd3dImmediateContext->ClearRenderTargetView(DXUTGetD3D11RenderTargetView(), Colors::Black);
	pd3dImmediateContext->ClearDepthStencilView(DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	//GUI 그리기
	g_GUI.OnRender(fElapsedTime);



	// 그림그릴 코드 작성...
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
	// 뒷 정리 코드 작성...
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
    // 디바이스 생성후 삭제를 안하면, 경고
#ifdef _DEBUG
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// 업데이트
	//DXUTSetCallbackFrameMove();
	// 키보드
	//DXUTSetCallbackKeyboard();
	// 메시지 프로시져
	DXUTSetCallbackMsgProc(MsgProc);


	DXUTSetCallbackD3D11DeviceCreated(OnD3D11DeviceCreated);
	// 화면 크기 변환, 이동 , 전환 호출되는 함수
	DXUTSetCallbackD3D11SwapChainResized(OnD3D11ResizedSwapChain);
	DXUTSetCallbackD3D11FrameRender(OnD3D11FrameRender);
	// 화면 크기 변환, 이동 , 전환 호출되는 함수
	DXUTSetCallbackD3D11SwapChainReleasing(OnD3D11ReleasingSwapChain);
	DXUTSetCallbackD3D11DeviceDestroyed(OnD3D11DeviceDestroyed);
    

	// UI초기화 코드 작성
	GUIInit();

    DXUTInit( true, true, nullptr );
    DXUTSetCursorSettings( true, true );
    DXUTCreateWindow( L"002 GUI" );
    DXUTCreateDevice( D3D_FEATURE_LEVEL_10_0, true, 800, 600 );
    DXUTMainLoop();
	
    return DXUTGetExitCode();
}
