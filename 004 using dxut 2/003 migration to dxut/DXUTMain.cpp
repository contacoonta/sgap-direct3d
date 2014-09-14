/*
	Mesh 클래스를 만들기 ]  with DXUT

	- 1. Box 버텍스 정보 만들기
	- 2. VertexBuffer , IndexBuffer 만들기
	- 3. Shader 를 만들고 연결하기
	- 4. 화면 Rendering
*/


#include "DXUT.h"
#include "SDKmisc.h"
#include "DXUTcamera.h"

#include "CompileShader.h"
#include "Mesh.h"


#pragma warning( disable : 4100 )

#include <DirectXMath.h>
using namespace DirectX;


CModelViewerCamera	g_camera;

CompileShader*		g_shader = nullptr;
Mesh*				g_mesh = nullptr;


HRESULT CALLBACK OnD3D11DeviceCreated(_In_ ID3D11Device* pd3dDevice, _In_ const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, _In_opt_ void* pUserContext)
{
	// 초기화 코드 작성...
	
	//g_shader = new CompileShader(L"Shaders\\lightShader.fx");
	//g_shader->BuildFromFile;

	CompileShader::Create(&g_shader, L"Shaders\\lightShader.fx");

	XMMATRIX mtrans = XMMatrixTranslation(1.0f, 0.0f, 0.0f);

	g_mesh = new Mesh;
	g_mesh->setWorld(mtrans);


	return S_OK;
}


HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, 
										  void* pUserContext )
{
	HRESULT hr;


	//view 매트릭스 설정
	XMVECTOR eye		= { 0.0f, 4.0f, -5.0f, 0.0f };
	XMVECTOR lookat		= { 0.0f, 0.0f, 0.0f, 0.0f };
	g_camera.SetViewParams(eye, lookat);	

	//proj 매트릭스 설정
	float fratio = pBackBufferSurfaceDesc->Width / pBackBufferSurfaceDesc->Height;
	g_camera.SetProjParams(XM_PI / 4, fratio, 0.1f, 1000.0f);
	g_camera.SetWindow(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
	g_camera.SetButtonMasks(MOUSE_LEFT_BUTTON, MOUSE_WHEEL, MOUSE_RIGHT_BUTTON);
	
    return S_OK;
}


void CALLBACK OnFrameMove(_In_ double fTime, _In_ float fElapsedTime, _In_opt_ void* pUserContext)
{
	
	g_camera.FrameMove(fElapsedTime);

}

void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice,
				ID3D11DeviceContext* pd3dImmediateContext,
				double fTime, float fElapsedTime, void* pUserContext)
{
	pd3dImmediateContext->ClearRenderTargetView(DXUTGetD3D11RenderTargetView(), Colors::DeepSkyBlue);
	pd3dImmediateContext->ClearDepthStencilView(DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	if (g_mesh)
	{
		CONSTANTBUFFER cb;
		ZeroMemory(&cb, sizeof(CONSTANTBUFFER));
		XMStoreFloat4x4(&cb.world, XMMatrixTranspose(g_mesh->getWorld()));
		XMStoreFloat4x4(&cb.view, XMMatrixTranspose(g_camera.GetViewMatrix()));
		XMStoreFloat4x4(&cb.projection, XMMatrixTranspose(g_camera.GetProjMatrix()));
		cb.litDir = XMFLOAT4(0.2f, 1.5f, -1.0f, 1.0f);
		cb.litCol = XMFLOAT4(0.7f, 0.7f, 0.6f, 1.0f);

		g_shader->RenderPrepare(&cb);
		g_mesh->Render();
	}
}

void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext)
{
	
}

void CALLBACK OnD3D11DeviceDestroyed(void* pUserContext)
{

	SAFE_DELETE(g_mesh);

	CompileShader::Delete(&g_shader);

}


LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{

	g_camera.HandleMessages(hWnd, uMsg, wParam, lParam);

    return 0;
}



int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
    // 디바이스 생성후 삭제를 안하면, 경고
#ifdef _DEBUG
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// 업데이트
	DXUTSetCallbackFrameMove(OnFrameMove);
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
    

    DXUTInit( true, true, nullptr );
    DXUTSetCursorSettings( true, true );
    DXUTCreateWindow( L"003 Migration to DXUT" );
    DXUTCreateDevice( D3D_FEATURE_LEVEL_10_0, true, 800, 600 );
    DXUTMainLoop();
	
    return DXUTGetExitCode();
}
