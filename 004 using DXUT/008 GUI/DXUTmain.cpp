/*
	1. DXUT 의 GUI 를 활용하기.
*/

#include "DXUT.h"
#include "DXUTgui.h"
#include "SDKmisc.h"

#include <string>
#include <wchar.h>

CDXUTDialogResourceManager	g_ResourceManager;
CDXUTTextHelper*			g_pTxtHelper = NULL;
CDXUTDialog					g_GUI;


#define GUI_BUTTON		2
#define GUI_SLIDER		3

#define GUI_STATIC1		101
#define GUI_STATIC2		102

/*
	GUI 만들기
*/
VOID CALLBACK OnGUIEvent(UINT nEvent, int nControlID, 
								CDXUTControl* pControl, void* pUserContext)
{
	UINT u = 0;

	switch (nControlID)
	{
	case GUI_BUTTON:		
		DXUTToggleFullScreen();
		break;

	case GUI_SLIDER:
	{
					   
		CDXUTStatic *pstatic = g_GUI.GetStatic(GUI_STATIC2);
		if (pstatic)
		{
			CDXUTSlider* pslider = g_GUI.GetSlider(GUI_SLIDER);
			wchar_t str[30] = {};
			auto val = pslider->GetValue();
			swprintf(str, L"%f", (float)val);
			pstatic->SetText(str);
		}
		break;	
	}
	default:
		break;
	}
}

void GUIInit()
{
	g_GUI.Init(&g_ResourceManager);
	g_GUI.SetCallback(OnGUIEvent);

	g_GUI.AddStatic(GUI_STATIC1, L"GUI HELLO~!", 10, 50, 100, 50);
	g_GUI.AddButton(GUI_BUTTON, L"BUTTON", 10, 100, 100, 50);
	g_GUI.AddSlider(GUI_SLIDER, 10, 150, 300, 20);

	g_GUI.AddStatic(GUI_STATIC2, L"GUAGE", 10, 200, 100, 50);

	
//과제 ] uidialog 로 texture 출력해보기
	/*
	int idx = g_ResourceManager.AddTexture(L"tile01.dds");
	g_ResourceManager.CreateTexture11(idx);
	g_ResourceManager.GetTextureNode(idx);

	g_GUI.DrawSprite();
	*/

}



void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{

	// GUI STATIC 에다가 FrameRate 정보를 출력.
	CDXUTStatic * pstatic = g_GUI.GetStatic(GUI_STATIC1);
	if (pstatic)
	{
		pstatic->SetText(DXUTGetFrameStats(true));
	}	

}

LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext)
{
	*pbNoFurtherProcessing = g_ResourceManager.MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;
	
	*pbNoFurtherProcessing = g_GUI.MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;

	return 0;
}

void CALLBACK OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
{
	if (bAltDown)
	{
		if (bKeyDown && nChar == 0x46 )
		{
			DXUTToggleFullScreen();
		}
	}
}

void CALLBACK OnMouse(bool bLeftButtonDown, bool bRightButtonDown, 
					bool bMiddleButtonDown, bool bSideButton1Down,
					bool bSideButton2Down, int nMouseWheelDelta,
					int xPos, int yPos, void* pUserContext)
{

}

bool CALLBACK OnDeviceChanging(DXUTDeviceSettings* pDeviceSettings, void* pUserContext)
{
	return true;
}

bool CALLBACK OnDeviceRemoved(void* pUserContext)
{
	return true;
}


bool CALLBACK IsDeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo,
							UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
				DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext)
{
	return true;
}

HRESULT CALLBACK OnCreateDevice(ID3D11Device* pd3dDevice, 
							const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
							void* pUserContext)
{

	ID3D11DeviceContext* pdevcon = DXUTGetD3D11DeviceContext();
	g_ResourceManager.OnD3D11CreateDevice(pd3dDevice, pdevcon);
	g_pTxtHelper = new CDXUTTextHelper(pd3dDevice, pdevcon, &g_ResourceManager, 15);


	return S_OK;
}

HRESULT CALLBACK OnResizedSwapChain(ID3D11Device* pd3dDevice, 
							IDXGISwapChain* pSwapChain,
							const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
							void* pUserContext)
{
	g_ResourceManager.OnD3D11ResizedSwapChain(pd3dDevice, pBackBufferSurfaceDesc);
	
	return S_OK;
}

void CALLBACK OnFrameRender(ID3D11Device* pd3dDevice, 
							ID3D11DeviceContext* pd3dImmediateContext,
							double fTime, float fElapsedTime, void* pUserContext)
{
	float ClearColor[4] = { 0.176f, 0.196f, 0.667f, 0.0f };

	ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
	ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
	pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
	pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);



	/*
		GUI render
	*/
	g_GUI.OnRender(fElapsedTime);


	g_pTxtHelper->Begin();
	{
		g_pTxtHelper->SetInsertionPos(10, 10);
		g_pTxtHelper->SetForegroundColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

		g_pTxtHelper->DrawTextLine(DXUTGetFrameStats(true));
	}
	g_pTxtHelper->End();

}

void CALLBACK OnReleasingSwapChain(void* pUserContext)
{
	g_ResourceManager.OnD3D11ReleasingSwapChain();
}

void CALLBACK OnDestroyDevice(void* pUserContext)
{
	g_ResourceManager.OnD3D11DestroyDevice();
	SAFE_DELETE(g_pTxtHelper);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
								LPWSTR lpCmdLine, int nCmdShow)
{
	/*
		기본적인 DXUT Callback 연결
	*/
	DXUTSetCallbackFrameMove(OnFrameMove);
	DXUTSetCallbackKeyboard(OnKeyboard);
	DXUTSetCallbackMouse(OnMouse);
	DXUTSetCallbackMsgProc(MsgProc);
	DXUTSetCallbackDeviceChanging(OnDeviceChanging);
	DXUTSetCallbackDeviceRemoved(OnDeviceRemoved);

	/*
		D3D 각종 인터페이스를 Callback 으로 연결
	*/
	DXUTSetCallbackD3D11DeviceAcceptable(IsDeviceAcceptable);
	DXUTSetCallbackD3D11DeviceCreated(OnCreateDevice);
	DXUTSetCallbackD3D11SwapChainResized(OnResizedSwapChain);
	DXUTSetCallbackD3D11FrameRender(OnFrameRender);
	DXUTSetCallbackD3D11SwapChainReleasing(OnReleasingSwapChain);
	DXUTSetCallbackD3D11DeviceDestroyed(OnDestroyDevice);
	
	/*
		윈도 생성 및 디바이스 생성
	*/
	DXUTInit(true, true, NULL);
	DXUTSetCursorSettings(true, true);

	GUIInit();

	DXUTCreateWindow(L"008 GUI");
	DXUTCreateDevice(D3D_FEATURE_LEVEL_10_0, true, 1024, 768);
	
	DXUTMainLoop(); 

	return DXUTGetExitCode();
}