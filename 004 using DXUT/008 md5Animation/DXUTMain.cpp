/*
	Mesh Ŭ������ ����� ]  with DXUT

	- 1. Box ���ؽ� ���� �����
	- 2. VertexBuffer , IndexBuffer �����
	- 3. Shader �� ����� �����ϱ�
	- 4. ȭ�� Rendering
*/


#include "DXUT.h"
#include "SDKmisc.h"
#include "DXUTcamera.h"
#include <DirectXMath.h>

#include "CompileShader.h"
#include "Transform.h"
#include "Mesh.h"
#include "MeshStatic.h"
#include "MeshSkeletal.h"
#include "LoaderObj.h"
#include "LoaderMd5.h"


#pragma warning( disable : 4100 )


CModelViewerCamera	g_camera;

CompileShader*		g_shader = nullptr;

Mesh*				g_ground = nullptr;
Mesh*				g_zealot = nullptr;
XMMATRIX			g_matLight;


/*
	Parent = S * R * T ;
	Child = (S * R * T) * Parent;
*/


HRESULT CALLBACK OnD3D11DeviceCreated(_In_ ID3D11Device* pd3dDevice, _In_ const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, _In_opt_ void* pUserContext)
{
	HRESULT hr = S_OK;

	// �ʱ�ȭ �ڵ� �ۼ�...

	CompileShader::Create(&g_shader, L"Shaders\\lightShader.fx");

	LoaderObj loaderobj;
	g_ground = loaderobj.BuildMeshFromFile(L"Models\\ground.obj", L"Textures\\grass.jpg");

	LoaderMd5 loadermd5;
	// ���� �޽�
	g_zealot = loadermd5.BuildMeshFromFile(L"Models\\zealot.md5mesh");
	g_zealot->setPosition(XMFLOAT3(0.0f, 0.25f, 0.0f));
	// ���� �ִϸ��̼�
	loadermd5.BuildAnimationFromFile(L"Models\\zealot_idle.md5anim", g_zealot);
	loadermd5.BuildAnimationFromFile(L"Models\\zealot_walk.md5anim", g_zealot);
	loadermd5.BuildAnimationFromFile(L"Models\\zealot_turnleft.md5anim", g_zealot);
	loadermd5.BuildAnimationFromFile(L"Models\\zealot_turnright.md5anim", g_zealot);
	loadermd5.BuildAnimationFromFile(L"Models\\zealot_attack1.md5anim", g_zealot);
	loadermd5.BuildAnimationFromFile(L"Models\\zealot_attack2.md5anim", g_zealot);
	
	return S_OK;
}


HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, 
										  void* pUserContext )
{
	HRESULT hr;


	//view ��Ʈ���� ����
	XMVECTOR eye		= { 5.0f, 5.0f, -5.0f, 0.0f };
	XMVECTOR lookat		= { 0.0f, 0.0f, 0.0f, 0.0f };
	g_camera.SetViewParams(eye, lookat);	

	//proj ��Ʈ���� ����
	float fratio = pBackBufferSurfaceDesc->Width / pBackBufferSurfaceDesc->Height;
	g_camera.SetProjParams(XM_PI / 4, fratio, 0.1f, 1000.0f);
	g_camera.SetWindow(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
	g_camera.SetButtonMasks(MOUSE_LEFT_BUTTON, MOUSE_WHEEL, MOUSE_RIGHT_BUTTON);
	
    return S_OK;
}


void CALLBACK OnFrameMove(_In_ double fTime, _In_ float fElapsedTime, _In_opt_ void* pUserContext)
{

	// �յ�
	if (GetAsyncKeyState('W') & 0x8000 || GetAsyncKeyState('w') & 0x8000)
	{
		g_zealot->moveForward(5.0f * fElapsedTime);
	}
	if (GetAsyncKeyState('S') & 0x8000 || GetAsyncKeyState('s') & 0x8000)
	{
		g_zealot->moveForward(-5.0f * fElapsedTime);
	}

	if (GetAsyncKeyState('Q') & 0x8000 || GetAsyncKeyState('q') & 0x8000)
	{
		g_zealot->moveStrafe(5.0f * fElapsedTime);
	}
	if (GetAsyncKeyState('E') & 0x8000 || GetAsyncKeyState('e') & 0x8000)
	{
		g_zealot->moveStrafe(-5.0f * fElapsedTime);
	}

	// �¿� ȸ��
	if (GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState('a') & 0x8000)
	{
		g_zealot->rotateYaw(-5.0f * fElapsedTime);
	}
	if (GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState('d') & 0x8000)
	{
		g_zealot->rotateYaw(5.0f * fElapsedTime);
	}


	//Ÿ�� �ٶ󺸱�
	if (GetAsyncKeyState('R') & 0x8000 || GetAsyncKeyState('r') & 0x8000)
	{
		XMFLOAT3 ftarget = XMFLOAT3(0, 0.25f, 0);
		g_zealot->setTarget(ftarget);
	}
	

	g_zealot->Update(fElapsedTime);
	
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
	XMMATRIX mat = XMLoadFloat4x4(&(g_ground->getWorld()));
	XMStoreFloat4x4(&cb.world, XMMatrixTranspose(mat));
	XMStoreFloat4x4(&cb.view, XMMatrixTranspose(g_camera.GetViewMatrix()));
	XMStoreFloat4x4(&cb.projection, XMMatrixTranspose(g_camera.GetProjMatrix()));

	/*
		1 0 0 0	Side (Right)
		0 1 0 0	Up
		0 0 1 0 Forward
		x y z 1 Position
	*/
	
	//1. XMMATRIX -> FLOAT4X4 
	XMVECTOR v = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR litpos = XMVector4Transform(v, g_matLight);
	litpos = XMVector4Normalize(litpos);
	XMStoreFloat4(&cb.litDir, litpos);
	cb.litCol = XMFLOAT4(0.7f, 0.7f, 0.6f, 1.0f);

	g_shader->RenderPrepare(&cb);
	g_ground->Render(g_shader);

	//marine
	mat = XMLoadFloat4x4(&(g_zealot->getWorld()));
	XMStoreFloat4x4(&cb.world, XMMatrixTranspose(mat));
	g_shader->RenderPrepare(&cb);
	g_zealot->Render(g_shader);
	
}

void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext)
{
	
}

void CALLBACK OnD3D11DeviceDestroyed(void* pUserContext)
{
	SAFE_DELETE(g_ground);
	SAFE_DELETE(g_zealot);

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
    // ����̽� ������ ������ ���ϸ�, ���
#ifdef _DEBUG
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// ������Ʈ
	DXUTSetCallbackFrameMove(OnFrameMove);
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
    

    DXUTInit( true, true, nullptr );
    DXUTSetCursorSettings( true, true );
    DXUTCreateWindow( L"006 MD5Mesh" );
    DXUTCreateDevice( D3D_FEATURE_LEVEL_10_0, true, 800, 600 );
    DXUTMainLoop();
	
    return DXUTGetExitCode();
}
