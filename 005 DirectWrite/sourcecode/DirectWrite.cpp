//--------------------------------------------------------------------------------------
// File: Basics.cpp
//
// This application demonstrates creating a Direct3D 11 device
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>

//direct write
#include <dwrite.h>
#include <d2d1.h>
#include <comdef.h>		// For _COM_SMARTPTR_TYPEDEF

#include "resource.h"

// link direct2d & direct write library
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib") 


//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_pd3dDevice = NULL;
ID3D11DeviceContext*    g_pImmediateContext = NULL;
IDXGISwapChain*         g_pSwapChain = NULL;
ID3D11RenderTargetView* g_pRenderTargetView = NULL;



// Direct Write
ID2D1Factory*			pD2DFactory_ = nullptr;
ID2D1HwndRenderTarget*	pRT_ = nullptr;
ID2D1SolidColorBrush*	pBlackBrush_ = nullptr;

IDWriteFactory*			pDWriteFactory_ = nullptr;
IDWriteTextFormat*		pTextFormat_ = nullptr;

const wchar_t* wszText_;
UINT32 cTextLength_;

RECT rc = { 0, 0, 800, 600 };


//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------

HRESULT InitDevice();
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    
	// 참조하지 않은 인자 , 컴파일 경고 무시하기
	UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

	// 윈도우 클래스 채우기
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"SGAP Direct3D";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	g_hInst = hInstance;
	
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow(L"SGAP Direct3D", L"Direct Draw", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
		NULL);
	if (!g_hWnd)
		return E_FAIL;

	ShowWindow(g_hWnd, nCmdShow);


	// 디바이스 초기화
    if( FAILED( InitDevice() ) )
    {
		// 실패하면 디바이스 정리
		if (g_pImmediateContext) g_pImmediateContext->ClearState();
		if (g_pRenderTargetView) g_pRenderTargetView->Release();
		if (g_pSwapChain) g_pSwapChain->Release();
		if (g_pImmediateContext) g_pImmediateContext->Release();
		if (g_pd3dDevice) g_pd3dDevice->Release();
        return 0;
    }


	// 
	// Create a Direct2D factory.

	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory_)))
	{
		return E_FAIL;
	}

	if (FAILED(DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory_))))
	{
		return E_FAIL;
	}

	wszText_ = L"Hello World using  DirectWrite!";
	//wszText_ = L"한글도 나옵니까 ?";
	cTextLength_ = (UINT32)wcslen(wszText_);


	pDWriteFactory_->CreateTextFormat(
											//L"Gabriola",                // Font family name.
											L"Arial",
											NULL,                       // Font collection (NULL sets it to use the system font collection).
											DWRITE_FONT_WEIGHT_REGULAR,
											DWRITE_FONT_STYLE_NORMAL,
											DWRITE_FONT_STRETCH_NORMAL,
											72.0f,
											L"en-us",
											&pTextFormat_
									);

	pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);


	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	// Create a Direct2D render target.
	pD2DFactory_->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(g_hWnd, size), &pRT_);
	pRT_->CreateSolidColorBrush( D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush_ );




    // Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
			// 화면 지우고 , 스왑체인 작동
			//float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
			//g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
			//g_pSwapChain->Present(0, 0);
			

			//
			D2D1_RECT_F layoutRect = D2D1::RectF(
				static_cast<FLOAT>(rc.left) ,/// 800,
				static_cast<FLOAT>(rc.top) ,/// 600,
				static_cast<FLOAT>(rc.right - rc.left) ,// / 800,
				static_cast<FLOAT>(rc.bottom - rc.top) // / 600
				);			

			pRT_->BeginDraw();

			pRT_->SetTransform(D2D1::IdentityMatrix());

			pRT_->Clear(D2D1::ColorF(D2D1::ColorF::AntiqueWhite));

			// Call the DrawText method of this class.
			pRT_->DrawText(
				wszText_,        // The string to render.
				cTextLength_,    // The string's length.
				pTextFormat_,    // The text format.
				layoutRect,       // The region of the window where the text will be rendered.
				pBlackBrush_     // The brush used to draw the text.
				);

			pRT_->EndDraw();
        }
    }

    //디바이스 정리
	if (g_pImmediateContext) g_pImmediateContext->ClearState();
	if (g_pRenderTargetView) g_pRenderTargetView->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (g_pImmediateContext) g_pImmediateContext->Release();
	if (g_pd3dDevice) g_pd3dDevice->Release();

	if (pRT_) pRT_->Release();
	if (pBlackBrush_) pBlackBrush_->Release();

    return ( int )msg.wParam;
}




//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

	D2D1_RECT_F layoutRect = D2D1::RectF(
		static_cast<FLOAT>(rc.left) ,/// 800,
		static_cast<FLOAT>(rc.top) ,/// 600,
		static_cast<FLOAT>(rc.right - rc.left) ,// / 800,
		static_cast<FLOAT>(rc.bottom - rc.top) // / 600
		);

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );

			//

			pRT_->BeginDraw();

			pRT_->SetTransform(D2D1::IdentityMatrix());

			pRT_->Clear(D2D1::ColorF(D2D1::ColorF::AntiqueWhite));

			// Call the DrawText method of this class.
			pRT_->DrawText(
				wszText_,        // The string to render.
				cTextLength_,    // The string's length.
				pTextFormat_,    // The text format.
				layoutRect,       // The region of the window where the text will be rendered.
				pBlackBrush_     // The brush used to draw the text.
				);

			pRT_->EndDraw();

            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( g_hWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = NULL;
    hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
        return hr;

    hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;

    g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, NULL );

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pImmediateContext->RSSetViewports( 1, &vp );

    return S_OK;
}
