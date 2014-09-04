
#include "DXUT.h"
#include "stdafx.h"
#include "FW1Precompiled.h"

#include <string>

#include "DXUTcamera.h"
#include "SDKmisc.h"

#include "VertexTypes.h"
#include "Effects.h"
#include "PrimitiveBatch.h"
#include "GeometricPrimitive.h"
#include "Input.h"
#include "DwriteText.h"


#pragma warning( disable : 4100 )
using namespace DirectX;


CModelViewerCamera	g_camera;
Input*				g_input;
DwriteText*			g_dwtext;

ID3D11InputLayout*										g_pBatchInputLayout = nullptr;
std::unique_ptr<BasicEffect>                            g_BatchEffect;
std::unique_ptr<PrimitiveBatch<VertexPositionColor>>    g_Batch;
std::unique_ptr<GeometricPrimitive>                     g_ShapeDir;
std::unique_ptr<GeometricPrimitive>                     g_ShapePos;

void DrawGrid(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color);
void DrawCenterGrid(FXMVECTOR yAxis1, FXMVECTOR yAxis2);

bool CALLBACK IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
									DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext)
{
	return true;
}


HRESULT CALLBACK OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
	HRESULT hr;

	g_input = new Input;
	g_dwtext = new DwriteText;

	g_ShapePos = GeometricPrimitive::CreateCube(DXUTGetD3D11DeviceContext(), 0.1f, false);
	g_ShapeDir = GeometricPrimitive::CreateOctahedron(DXUTGetD3D11DeviceContext(), 0.1f, false);

	g_Batch.reset(new PrimitiveBatch<VertexPositionColor>(DXUTGetD3D11DeviceContext()));
	g_BatchEffect.reset( new BasicEffect( pd3dDevice ) );
    g_BatchEffect->SetVertexColorEnabled(true);
	{
        void const* shaderByteCode;
        size_t byteCodeLength;

        g_BatchEffect->GetVertexShaderBytecode( &shaderByteCode, &byteCodeLength );

        hr = pd3dDevice->CreateInputLayout( VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
                                            shaderByteCode, byteCodeLength, &g_pBatchInputLayout );
        if( FAILED( hr ) )
            return hr;
    }

	WCHAR str[MAX_PATH];
	

	static const XMVECTOR eye = { 2.0f, 5.0f, -5.0f, 0.f };
	static const XMVECTOR lookat = { 0.0f, 0.0f, 0.0f, 0.f };
	g_camera.SetViewParams(eye, lookat);

	return S_OK;
}


HRESULT CALLBACK OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
									const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
	HRESULT hr;

	float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
	g_camera.SetProjParams(XM_PI / 4, fAspectRatio, 0.001f, 1000.0f);
	g_camera.SetWindow(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
	g_camera.SetButtonMasks(MOUSE_LEFT_BUTTON, MOUSE_WHEEL, MOUSE_RIGHT_BUTTON);

	return S_OK;
}

void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
	g_input->Update();

	if (g_input->isKeyPressed(DIK_ESCAPE))
		DXUTShutdown();

	g_camera.FrameMove(fElapsedTime);
}


void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime,
	float fElapsedTime, void* pUserContext)
{
	pd3dImmediateContext->ClearRenderTargetView(DXUTGetD3D11RenderTargetView(), Colors::Black);
	pd3dImmediateContext->ClearDepthStencilView(DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0, 0);

	XMMATRIX mworld = XMMatrixIdentity();
	XMMATRIX mview = g_camera.GetViewMatrix();
	XMMATRIX mproj = g_camera.GetProjMatrix();

	g_BatchEffect->SetView(mview);
	g_BatchEffect->SetProjection(mproj);

	// Draw Grids
	const XMVECTORF32 xaxis = { 20.f, 0.f, 0.f };
	const XMVECTORF32 yaxis = { 0.f, 0.f, 20.f };
	DrawGrid(xaxis, yaxis, g_XMZero, 4, 4, Colors::Azure);
	

	// Draw Font
	g_dwtext->Render(L"ÇÁ·¹ÀÓ = ", DXUTGetFPS(), 10, 10);
	
	XMVECTOR worldPos;
	XMVECTOR worldDir;

	
	g_input->getMousePosWorld(worldPos, worldDir, g_camera.GetWorldMatrix(), g_camera.GetViewMatrix(), g_camera.GetProjMatrix());

	/*
		POSITION
	*/
	WCHAR strformat[256] = {};
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, worldPos);
	swprintf(strformat, L"pos = %f , %f , %f", pos.x, pos.y, pos.z);
	g_dwtext->Render(strformat, 0, 10, 30);


	/*
		DIRECTION
	*/
	XMFLOAT3 dir;
	XMStoreFloat3(&dir, worldDir);
	swprintf(strformat, L"dir = %f , %f , %f", dir.x, dir.y, dir.z);
	g_dwtext->Render(strformat, 0, 10, 50);


	/*
		CURSOR POSITION
	*/
	POINT pt = g_input->getMousePos();
	swprintf(strformat, L"cursor = %d , %d", pt.x, pt.y );
	g_dwtext->Render(strformat, 0, 10, 70);



	const XMVECTORF32 yaxis1 = { pos.x, pos.y, pos.z };
	const XMVECTORF32 yaxis2 = { dir.x, dir.y, dir.z };
	DrawCenterGrid(yaxis1, yaxis2);

	XMMATRIX wpos = XMMatrixTranslation(pos.x, pos.y, pos.z);
	g_ShapePos->Draw(wpos, mview, mproj, Colors::Red);

	XMMATRIX wdir = XMMatrixTranslation(dir.x, dir.y, dir.z);
	g_ShapeDir->Draw(wdir, mview, mproj, Colors::MediumTurquoise);	
	
}


void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext)
{
}

void CALLBACK OnD3D11DestroyDevice(void* pUserContext)
{
	g_ShapePos.reset();
	g_ShapeDir.reset();
	g_BatchEffect.reset();
	g_Batch.reset();
	SAFE_RELEASE(g_pBatchInputLayout);
	SAFE_DELETE(g_input);
	SAFE_DELETE(g_dwtext);
	
}

bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext)
{
	return true;
}



LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext)
{
	g_camera.HandleMessages(hWnd, uMsg, wParam, lParam);

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
    DXUTCreateWindow( L"003 DxUTK - DirectWrite" );
    DXUTCreateDevice( D3D_FEATURE_LEVEL_10_0, true, 800, 600 );

    DXUTMainLoop();

    return DXUTGetExitCode();
}


/*
*/
void DrawGrid( FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color )
{
    auto context = DXUTGetD3D11DeviceContext();
    g_BatchEffect->Apply( context );

    context->IASetInputLayout( g_pBatchInputLayout );

    g_Batch->Begin();

    xdivs = std::max<size_t>( 1, xdivs );
    ydivs = std::max<size_t>( 1, ydivs );

    for( size_t i = 0; i <= xdivs; ++i )
    {
        float fPercent = float(i) / float(xdivs);
        fPercent = ( fPercent * 2.0f ) - 1.0f;
        XMVECTOR vScale = XMVectorScale( xAxis, fPercent );
        vScale = XMVectorAdd( vScale, origin );

        VertexPositionColor v1( XMVectorSubtract( vScale, yAxis ), color );
        VertexPositionColor v2( XMVectorAdd( vScale, yAxis ), color );
        g_Batch->DrawLine( v1, v2 );
    }

    for( size_t i = 0; i <= ydivs; i++ )
    {
        FLOAT fPercent = float(i) / float(ydivs);
        fPercent = ( fPercent * 2.0f ) - 1.0f;
        XMVECTOR vScale = XMVectorScale( yAxis, fPercent );
        vScale = XMVectorAdd( vScale, origin );

        VertexPositionColor v1( XMVectorSubtract( vScale, xAxis ), color );
        VertexPositionColor v2( XMVectorAdd( vScale, xAxis ), color );
        g_Batch->DrawLine( v1, v2 );
    }

    g_Batch->End();
}


void DrawCenterGrid(FXMVECTOR yAxis1, FXMVECTOR yAxis2)
{
	VertexPositionColor v1(yAxis1, Colors::Red);
	VertexPositionColor v2(yAxis2, Colors::Red);

	DXUTGetD3D11DeviceContext()->IASetInputLayout(g_pBatchInputLayout);
	
	g_BatchEffect->Apply(DXUTGetD3D11DeviceContext());

    g_Batch->Begin();
    g_Batch->DrawLine( v1, v2 );
    g_Batch->End();
}