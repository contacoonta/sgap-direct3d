﻿
#include "DXUT.h"
#include "FW1Precompiled.h"
#include "SDKmisc.h"
#include "DXUTcamera.h"
#include <DirectXMath.h>
#include "DwriteText.h"

#include "CompileShader.h"
#include "Transform.h"
#include "DInput.h"
#include "Camera.h"
#include "Mesh.h"
#include "MeshStatic.h"
#include "MeshSkeletal.h"
#include "LoaderObj.h"
#include "LoaderMd5.h"


#pragma warning( disable : 4100 )


Camera*				g_camera = nullptr;
CompileShader*		g_shader = nullptr;

Mesh*				g_ground = nullptr;
Mesh*				g_box = nullptr;

Mesh*				g_zealot = nullptr;
XMMATRIX			g_matLight;


/*
	Parent = S * R * T ;
	Child = (S * R * T) * Parent;
*/


HRESULT CALLBACK OnD3D11DeviceCreated(_In_ ID3D11Device* pd3dDevice, _In_ const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, _In_opt_ void* pUserContext)
{
	HRESULT hr = S_OK;

	TEXT.SetFontSize(16.0f);
	TEXT.SetMaxLines(30);

	g_camera = new Camera;

	// 초기화 코드 작성...
	CompileShader::Create(&g_shader, L"Shaders\\lightShader.fx");

	LoaderObj loaderobj;
	g_ground = loaderobj.BuildMeshFromFile(L"Models\\ground.obj", L"Textures\\ground_weeds.jpg");
	g_box = loaderobj.BuildCube();
	g_box->setScale(XMFLOAT3(0.2f, 0.2f, 0.2f));

	LoaderMd5 loadermd5;
	// 질럿 메시
	g_zealot = loadermd5.BuildMeshFromFile(L"Models\\zealot.md5mesh");
	g_zealot->setPosition(XMFLOAT3(0.0f, 0.25f, 0.0f));
	// 질럿 애니메이션
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
	
	g_camera->setOffset(XMFLOAT3(0.5f, 3.0f, -1.0f));
	g_camera->computeProjMat();
	
    return S_OK;
}


void CALLBACK OnFrameMove(_In_ double fTime, _In_ float fElapsedTime, _In_opt_ void* pUserContext)
{
	// 앞뒤
	if ( INPUT.isKeyPressed(DIK_W) )
	{
		g_zealot->moveForward(5.0f * fElapsedTime);
	}
	if ( INPUT.isKeyPressed(DIK_S) )
	{
		g_zealot->moveForward(-5.0f * fElapsedTime);
	}

	if (INPUT.isKeyPressed(DIK_Q))
	{
		g_zealot->moveStrafe(-5.0f * fElapsedTime);
	}
	if (INPUT.isKeyPressed(DIK_E))
	{
		g_zealot->moveStrafe(5.0f * fElapsedTime);
	}

	// 좌우 회전
	if (INPUT.isKeyPressed(DIK_A))
	{
		g_zealot->rotateYaw(-5.0f * fElapsedTime);
	}
	if (INPUT.isKeyPressed(DIK_D))
	{
		g_zealot->rotateYaw(5.0f * fElapsedTime);
	}

	//타겟 바라보기
	if (INPUT.isKeyPressed(DIK_R))
	{
		XMFLOAT3 ftarget = XMFLOAT3(0, 0.25f, 0);
		g_zealot->setTarget(ftarget);
	}

	// 마우스 버튼을 누르면 , 피킹 레이 발사
	if (INPUT.isLBtnDown())
	{
		XMFLOAT3 ray = XMFLOAT3(0, 0, 0);
		XMFLOAT3 dir = XMFLOAT3(0, 0, 0);

		// 카메라를 기준으로 피킹 레이 위치와 방향 가져오기
		g_camera->Pick(ray, dir);

		TEXT.AddString(L" ");
		TEXT.AddString(L"Ray Origin ] ");
		TEXT.AddString(L"X = " + std::to_wstring(ray.x)
			+ L" Y = " + std::to_wstring(ray.y)
			+ L" Z = " + std::to_wstring(ray.z));

		TEXT.AddString(L"Ray Direction ] ");
		TEXT.AddString(L"X = " + std::to_wstring(dir.x)
			+ L" Y = " + std::to_wstring(dir.y)
			+ L" Z = " + std::to_wstring(dir.z));
		
		// 피킹 레이 시작점
		XMVECTOR r = XMLoadFloat3(&ray);
		// 피킹 레이 방향
		XMVECTOR d = XMLoadFloat3(&dir);
		// 피킹 레이의 끝 점 구하기 = 시작점 + ( 방향 * 길이 )
		XMVECTOR p = r + ( d * 6.0f );

		g_box->setPositionXM(p);

		XMFLOAT3 boxpos = g_box->getPosition();

		// 질럿이 땅에서 0.25f 만큼 위에 올라온채로 초기화 했기에, 0.25f 로 고정
		g_zealot->setTarget(XMFLOAT3(boxpos.x, 0.25f, boxpos.z));
	}
	
	g_zealot->Update(fElapsedTime);	
	
	g_camera->setEye(g_zealot->getPosition());
	g_camera->setLookat(g_zealot->getPosition());

}

void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice,
				ID3D11DeviceContext* pd3dImmediateContext,
				double fTime, float fElapsedTime, void* pUserContext)
{
	pd3dImmediateContext->ClearRenderTargetView(DXUTGetD3D11RenderTargetView(), Colors::DarkBlue);
	pd3dImmediateContext->ClearDepthStencilView(DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		

	/*
		1 0 0 0	Side (Right)
		0 1 0 0	Up
		0 0 1 0 Forward
		x y z 1 Position
	*/
	
	CONSTANTBUFFER cb;
	ZeroMemory(&cb, sizeof(CONSTANTBUFFER));
	
	XMStoreFloat4x4(&cb.view, XMMatrixTranspose(g_camera->getViewXM()));
	XMStoreFloat4x4(&cb.projection, XMMatrixTranspose(g_camera->getProjXM()));

	XMVECTOR v = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR litpos = XMVector4Transform(v, g_matLight);
	litpos = XMVector4Normalize(litpos);
	XMStoreFloat4(&cb.litDir, litpos);
	cb.litCol = XMFLOAT4(0.9f, 1.0f, 0.8f, 1.0f);

	/*
		그라운드 그리기
	*/
	XMStoreFloat4x4(&cb.world, XMMatrixTranspose(g_ground->getWorldXM()));
	g_shader->RenderPrepare(&cb);
	g_ground->Render(g_shader);


	/*
		박스 그리기
	*/
	XMStoreFloat4x4(&cb.world, XMMatrixTranspose(g_box->getWorldXM()));
	g_shader->RenderPrepare(&cb);
	g_box->Render(g_shader);

	/*
		질럿 그리기
	*/
	XMStoreFloat4x4(&cb.world, XMMatrixTranspose(g_zealot->getWorldXM()));
	g_shader->RenderPrepare(&cb);
	g_zealot->Render(g_shader);
	
	/*
		텍스트 출력
	*/
	TEXT.Render(L"FPS : ", DXUTGetFPS(), 10, 10);
	TEXT.RenderStrings();
}

void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext)
{
	
}

void CALLBACK OnD3D11DeviceDestroyed(void* pUserContext)
{
	INPUT.Release();

	SAFE_DELETE(g_ground);
	SAFE_DELETE(g_box);
	SAFE_DELETE(g_zealot);
	SAFE_DELETE(g_camera);
	CompileShader::Delete(&g_shader);
}


LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{

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
	// 메시지 프로시져
	DXUTSetCallbackMsgProc(MsgProc);


	DXUTSetCallbackD3D11DeviceCreated(OnD3D11DeviceCreated);
	DXUTSetCallbackD3D11SwapChainResized(OnD3D11ResizedSwapChain);
	DXUTSetCallbackD3D11SwapChainReleasing(OnD3D11ReleasingSwapChain);
	DXUTSetCallbackD3D11FrameRender(OnD3D11FrameRender);
	DXUTSetCallbackD3D11DeviceDestroyed(OnD3D11DeviceDestroyed);
    
    DXUTInit( true, true, nullptr );
    DXUTSetCursorSettings( true, true );
    DXUTCreateWindow( L"012 Picking Ray" );
    DXUTCreateDevice( D3D_FEATURE_LEVEL_10_0, true, 1024, 768 );
    DXUTMainLoop();
	
    return DXUTGetExitCode();
}
