
#include "DXUT.h"
#include "DXUTcamera.h"
#include "SDKmisc.h"

#include "Mesh.h"
#include "ObjLoader.h"
#include "Md5Loader.h"
#include "CompileShader.h"

#pragma warning( disable : 4100 )
using namespace DirectX;


CModelViewerCamera	g_camera;
CompileShader*		g_shader	= nullptr;
Mesh*				g_mesh		= nullptr;

Model3D									g_md5model;
std::vector<ID3D11ShaderResourceView*>	meshSRV;
std::vector<std::wstring>				textureNameArray;


bool CALLBACK IsD3D11DeviceAcceptable( const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
                                       DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    return true;
}

bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    return true;
}

HRESULT CALLBACK OnD3D11CreateDevice( ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
                                      void* pUserContext )
{
    HRESULT hr = S_OK;
	
	// Shader Layout ¼³Á¤
	hr = CompileShader::Create(&g_shader, L"shaders\\md5shader.fx");
	if (FAILED(hr))
		return hr;

	//ObjLoader loader;
	//g_mesh = loader.BuildMeshFromFile(L"models\\marine.obj");

	/*XMMATRIX mat = XMMatrixTranslation(0, 0.0f, 0.0f);
	g_mesh->SetWorld(mat);*/


	md5Loader loadermd5;
	loadermd5.LoadMd5Model(L"models\\cylinder.md5mesh", g_md5model, meshSRV, textureNameArray);
		
	
	static const XMVECTOR eye = { 20.0f, 50.0f, -50.0f, 0.f };
	static const XMVECTOR lookat = { 0.0f, 1.0f, 0.0f, 0.f };
	g_camera.SetViewParams(eye, lookat);

    return S_OK;
}

HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{

	float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
	g_camera.SetProjParams(XM_PI / 4, fAspectRatio, 0.1f, 1000.0f);
	g_camera.SetWindow(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
	g_camera.SetButtonMasks(MOUSE_LEFT_BUTTON, MOUSE_WHEEL, MOUSE_RIGHT_BUTTON);


    return S_OK;
}

void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	g_camera.FrameMove(fElapsedTime);
	g_mesh->Update();
}

void CALLBACK OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
                                  double fTime, float fElapsedTime, void* pUserContext )
{
	pd3dImmediateContext->ClearRenderTargetView(DXUTGetD3D11RenderTargetView(), Colors::MidnightBlue);
	pd3dImmediateContext->ClearDepthStencilView(DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0, 0);
	
	XMMATRIX mview = g_camera.GetViewMatrix();
	XMMATRIX mproj = g_camera.GetProjMatrix();
	XMFLOAT4 LitDir = XMFLOAT4(0.577f, -0.577f, -0.577f, 1.0f);
	XMFLOAT4 LitCol = XMFLOAT4(0.5f, 0.6f, 0.5f, 1.0f);

	/*
		SHADER CONSTANT BUFFER
	*/
	ConstantBuffer cb;
	XMStoreFloat4x4(&(cb.world), XMMatrixTranspose(g_mesh->World()));
	XMStoreFloat4x4(&(cb.view), XMMatrixTranspose(mview));
	XMStoreFloat4x4(&(cb.projection), XMMatrixTranspose(mproj));
	cb.litDir = LitDir;
	cb.litCol = LitCol;

	/*g_shader->RenderPrepare(&cb);
	g_mesh->Render();*/


	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	for (int i = 0; i < g_md5model.numSubsets; i++)
	{
		//Set the grounds index buffer
		DXUTGetD3D11DeviceContext()->IASetIndexBuffer(g_md5model.subsets[i].indexBuff, DXGI_FORMAT_R32_UINT, 0);
		//Set the grounds vertex buffer
		DXUTGetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &g_md5model.subsets[i].vertBuff, &stride, &offset);

		//Set the WVP matrix and send it to the constant buffer in effect file
		//WVP = smilesWorld * camView * camProjection;
		//cbPerObj.WVP = XMMatrixTranspose(WVP);
		//cbPerObj.World = XMMatrixTranspose(smilesWorld);
		//cbPerObj.hasTexture = true;		// We'll assume all md5 subsets have textures
		//cbPerObj.hasNormMap = false;	// We'll also assume md5 models have no normal map (easy to change later though)
		//d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
		//d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
		//d3d11DevCon->PSSetConstantBuffers(1, 1, &cbPerObjectBuffer);
		/*d3d11DevCon->PSSetShaderResources(0, 1, &meshSRV[NewMD5Model.subsets[i].texArrayIndex]);
		d3d11DevCon->PSSetSamplers(0, 1, &CubesTexSamplerState);*/

		//DXUTGetD3D11DeviceContext()->RSSetState(RSCullNone);
		XMMATRIX mat = XMMatrixIdentity();
		XMStoreFloat4x4(&(cb.world), XMMatrixTranspose(mat));
		g_shader->RenderPrepare(&cb);
		DXUTGetD3D11DeviceContext()->DrawIndexed(g_md5model.subsets[i].indices.size(), 0, 0);
	}

}

void CALLBACK OnD3D11ReleasingSwapChain( void* pUserContext )
{
}

void CALLBACK OnD3D11DestroyDevice( void* pUserContext )
{
	delete g_mesh;

	for (int i = 0; i < g_md5model.numSubsets; i++)
	{
		g_md5model.subsets[i].indexBuff->Release();
		g_md5model.subsets[i].vertBuff->Release();
	}
	
	if (g_shader) CompileShader::Delete(&g_shader);
}

LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
	g_camera.HandleMessages(hWnd, uMsg, wParam, lParam);

    return 0;
}

void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
    if( bKeyDown )
    {
        switch( nChar )
        {
            case VK_F1: // Change as needed                
                break;
        }
    }
}

bool CALLBACK OnDeviceRemoved( void* pUserContext )
{
    return true;
}


/*
*/
int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
    // Enable run-time memory check for debug builds.
#ifdef _DEBUG
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // DXUT will create and use the best device
    // that is available on the system depending on which D3D callbacks are set below

    // Set general DXUT callbacks
    DXUTSetCallbackFrameMove( OnFrameMove );
    DXUTSetCallbackKeyboard( OnKeyboard );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
    DXUTSetCallbackDeviceRemoved( OnDeviceRemoved );

    // Set the D3D11 DXUT callbacks. Remove these sets if the app doesn't need to support D3D11
    DXUTSetCallbackD3D11DeviceAcceptable( IsD3D11DeviceAcceptable );
    DXUTSetCallbackD3D11DeviceCreated( OnD3D11CreateDevice );
    DXUTSetCallbackD3D11SwapChainResized( OnD3D11ResizedSwapChain );
    DXUTSetCallbackD3D11FrameRender( OnD3D11FrameRender );
    DXUTSetCallbackD3D11SwapChainReleasing( OnD3D11ReleasingSwapChain );
    DXUTSetCallbackD3D11DeviceDestroyed( OnD3D11DestroyDevice );

    // Perform any application-level initialization here

    DXUTInit( true, true, nullptr ); // Parse the command line, show msgboxes on error, no extra command line params
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"002 Md5Loader" );

    // Only require 10-level hardware or later
    DXUTCreateDevice( D3D_FEATURE_LEVEL_10_0, true, 800, 600 );
    DXUTMainLoop(); // Enter into the DXUT render loop

    // Perform any application-level cleanup here

    return DXUTGetExitCode();
}
