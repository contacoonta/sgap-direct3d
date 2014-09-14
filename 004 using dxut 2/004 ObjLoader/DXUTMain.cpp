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
#include <DirectXMath.h>

#include "CompileShader.h"
#include "Mesh.h"
#include "LoaderObj.h"


#pragma warning( disable : 4100 )


CModelViewerCamera	g_camera;

CompileShader*		g_shader = nullptr;
Mesh*				g_mesh = nullptr;
Mesh*				g_meshLight = nullptr;
XMMATRIX			g_matLight;


/*
	Parent = S * R * T ;
	Child = (S * R * T) * Parent;
*/


HRESULT CALLBACK OnD3D11DeviceCreated(_In_ ID3D11Device* pd3dDevice, _In_ const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, _In_opt_ void* pUserContext)
{
	HRESULT hr = S_OK;

	// 초기화 코드 작성...

	CompileShader::Create(&g_shader, L"Shaders\\lightShader.fx");
	hr = g_shader->CreateTextureFromFile(L"Textures\\grass.jpg");


	LoaderObj loader;	
	//g_mesh	= loader.BuildCube();
	g_mesh = loader.BuildMeshFromFile(L"Models\\ground.obj");
	g_meshLight = loader.BuildMeshFromFile(L"Models\\ground.obj");
	
	XMMATRIX mscale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX mtrans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMMATRIX world = mscale * mtrans;
	XMFLOAT4X4 fworld;

	//1. XMMATRIX -> FLOAT4X4 
	XMStoreFloat4x4(&fworld, world);	
	//2. FLOAT4X4 -> XMMATRIX
	//world = XMLoadFloat4x4(&fworld);
			
	g_mesh->setWorld(fworld);




	return S_OK;
}


HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, 
										  void* pUserContext )
{
	HRESULT hr;


	//view 매트릭스 설정
	XMVECTOR eye		= { 5.0f, 5.0f, -100.0f, 0.0f };
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
	static float rot = 0.0f;
	rot += fElapsedTime;
	XMMATRIX mscale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX mrot	= XMMatrixRotationRollPitchYaw(0.0f, 0.0f, rot);
	XMMATRIX mtrans = XMMatrixTranslation(20.0f, 50.0f, 0.0f);

	// gmesh 의 월드 매트릭스 -> XMMATRIX
	XMMATRIX mParent = XMLoadFloat4x4(&g_mesh->getWorld());

	g_matLight = (mscale * mtrans * mrot) * mParent;
	
	// 조명 메시 연결
	XMFLOAT4X4 mlit = XMFLOAT4X4();
	XMStoreFloat4x4(&mlit, g_matLight);
	g_meshLight->setWorld(mlit);


	g_camera.FrameMove(fElapsedTime);




}

void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice,
				ID3D11DeviceContext* pd3dImmediateContext,
				double fTime, float fElapsedTime, void* pUserContext)
{
	pd3dImmediateContext->ClearRenderTargetView(DXUTGetD3D11RenderTargetView(), Colors::DeepSkyBlue);
	pd3dImmediateContext->ClearDepthStencilView(DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	
	CONSTANTBUFFER cb;
	ZeroMemory(&cb, sizeof(CONSTANTBUFFER));
	XMMATRIX mat = XMLoadFloat4x4(&(g_mesh->getWorld()));	
	XMStoreFloat4x4(&cb.world, XMMatrixTranspose(mat));

	//cb.world =  g_mesh->getWorld();
	XMStoreFloat4x4(&cb.view, XMMatrixTranspose(g_camera.GetViewMatrix()));
	XMStoreFloat4x4(&cb.projection, XMMatrixTranspose(g_camera.GetProjMatrix()));

	/*
		1 0 0 0	Side (Right)
		0 1 0 0	Up
		0 0 1 0 Foward
		x y z 1 Position
	*/


	//1. XMMATRIX -> FLOAT4X4 
	XMVECTOR v = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR litpos = XMVector4Transform(v, g_matLight);
	litpos = XMVector4Normalize(litpos);
	XMStoreFloat4(&cb.litDir, litpos);
	cb.litCol = XMFLOAT4(0.7f, 0.7f, 0.6f, 1.0f);

	g_shader->RenderPrepare(&cb);
	g_mesh->Render();


	//
	mat = XMLoadFloat4x4(&(g_meshLight->getWorld()));
	XMStoreFloat4x4(&cb.world, XMMatrixTranspose(mat));
	g_shader->RenderPrepare(&cb);
	g_meshLight->Render();
	
}

void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext)
{
	
}

void CALLBACK OnD3D11DeviceDestroyed(void* pUserContext)
{
	SAFE_DELETE(g_mesh);
	SAFE_DELETE(g_meshLight);

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
