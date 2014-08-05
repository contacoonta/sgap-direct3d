/*
	TEXTURING

*/

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>

#include <D3Dcompiler.h>
#include <xnamath.h>


/*
	버텍스 구조체
*/
typedef struct SimpleVertex
{
	XMFLOAT3 pos;
	XMFLOAT3 nor;			
	XMFLOAT2 tex;			//텍스쳐 UV

}SIMPLEVERTEX;


/*
	상수버퍼 구조체 ( Constant Buffer )
*/
typedef struct CBNeverChanges
{
	XMMATRIX mview;

}CBNEVERCHANGES;

typedef struct CBChangeOnResize
{
	XMMATRIX mprojection;

}CBCHANGEONRESIZE;

typedef struct CBChangesEveryFrame
{
	XMMATRIX mworld;

	XMFLOAT4 litDir[2];		
	XMFLOAT4 litCol[2];		
	
	XMFLOAT4 colOutput;

}CBCHANGESEVERYFRAME;


//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_device = NULL;
ID3D11DeviceContext*    g_devcon = NULL;
IDXGISwapChain*         g_pSwapChain = NULL;
ID3D11RenderTargetView* g_rtBackbuffer = NULL;

ID3D11VertexShader*     g_pVertexShader = NULL;
ID3D11PixelShader*      g_pSolidPixelShader = NULL;
ID3D11InputLayout*      g_pVertexLayout = NULL;
ID3D11Buffer*           g_pVertexBuffer = NULL;

ID3D11Buffer*           g_pIndexBuffer = NULL;
//ID3D11Buffer*           g_pConstantBuffer = NULL;
XMMATRIX                g_World;
XMMATRIX                g_View;
XMMATRIX                g_Projection;

ID3D11DepthStencilView* g_pDepthStencilView = NULL;

ID3D11PixelShader*      g_pLightPixelShader = NULL;

// 기존 상수 버퍼를 3가지 경우의 상수 버퍼로 분리
ID3D11Buffer*           g_pCBNeverChanges = NULL;
ID3D11Buffer*           g_pCBChangeOnResize = NULL;
ID3D11Buffer*           g_pCBChangesEveryFrame = NULL;

// 텍스쳐 처리를 위한 인터페이스들
ID3D11ShaderResourceView*	g_pTextureRView = NULL;
ID3D11SamplerState*         g_pSamplerLinear = NULL;

XMFLOAT4                    g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);



//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitDevice();
void	ReleaseDevice();


HRESULT ComplieShaderFromFile(WCHAR* wFilename, LPCSTR strEntry, LPCSTR strShaderMdl, ID3DBlob** ppblob);
void	Render();

LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

	WCHAR classname[] = L"005 Light";

	 // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_WINLOGO );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
	wcex.lpszClassName = classname;
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_WINLOGO);
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 1024, 768 };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
	g_hWnd = CreateWindow(classname, classname, WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
                           NULL );
    if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );


    if( FAILED( InitDevice() ) )
    {
		ReleaseDevice();

        return 0;
    }

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

			Render();

        }
    }//while()

	ReleaseDevice();

    return ( int )msg.wParam;
}



//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{

    switch( message )
    {
	case WM_KEYDOWN :
		if (wParam == VK_UP)
		{
			
		}
		else if (wParam == VK_DOWN)
		{
			
		}

		if (wParam == VK_LEFT)
		{
			
		}
		else if (wParam == VK_RIGHT)
		{
			
		}
		break;

	case WM_CLOSE:
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
                                            D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_device, &g_featureLevel, &g_devcon );
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

    hr = g_device->CreateRenderTargetView( pBackBuffer, NULL, &g_rtBackbuffer );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;


	/*
		Depth Stencil 생성 및 연결
	*/

	// depth stencil 텍스쳐 설정
	D3D11_TEXTURE2D_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.Width = width;
	depthDesc.Height = height;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	//32bit , depth 24 bit, stencil 8 bit
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;
	
	// Device 로 depth stencil 용 텍스쳐 생성
	ID3D11Texture2D* pDepthStencil = NULL;
	hr = g_device->CreateTexture2D(&depthDesc, NULL, &pDepthStencil);
	if (FAILED(hr))
		return hr;
	
	// depth stencil view 생성
	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	ZeroMemory(&DSVDesc, sizeof(DSVDesc));
	DSVDesc.Format = depthDesc.Format;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Texture2D.MipSlice = 0;

	hr = g_device->CreateDepthStencilView(pDepthStencil, &DSVDesc, &g_pDepthStencilView);
	pDepthStencil->Release();

	if (FAILED(hr))
		return hr;
	
	g_devcon->OMSetRenderTargets(1, &g_rtBackbuffer, g_pDepthStencilView);


	/*
		VIEWPORT 설정
	*/
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_devcon->RSSetViewports( 1, &vp );

	

	/*
		VERTEX SHADER
	*/
	ID3DBlob* pVSblob = nullptr;
	// FX 파일 중 VertexShader 로드
	hr = ComplieShaderFromFile(L"textureShader.fx", "VS", "vs_4_0", &pVSblob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Load FX file FAILED", L"ERROR", MB_OK);
		return hr;
	}

	// Vertex Shader 생성
	hr = g_device->CreateVertexShader(pVSblob->GetBufferPointer(), pVSblob->GetBufferSize(), NULL, &g_pVertexShader);
	if (FAILED(hr))
	{
		pVSblob->Release();
		return hr;
	}



	/*
		VERTEX LAYOUT
	*/
	// Shader Layout 설정
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(layout);

	// Layout 구조체 정보를 바탕으로 VertexLayout 을 생성.
	hr = g_device->CreateInputLayout(layout, numElements, pVSblob->GetBufferPointer(), pVSblob->GetBufferSize(), &g_pVertexLayout);
	pVSblob->Release();
	if (FAILED(hr))
		return hr;

	// VertexLayout 을 device Context 에 설정.
	g_devcon->IASetInputLayout(g_pVertexLayout);




	/*
		PIXEL SHADER ( for LIGHT )
	*/
	ID3DBlob* pPSblob = nullptr;
	hr = ComplieShaderFromFile(L"textureShader.fx", "PSLight", "ps_4_0", &pPSblob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Load FX file FAILED", L"ERROR", MB_OK);
		return hr;
	}

	// LIGHT Pixel Shader 생성
	hr = g_device->CreatePixelShader(pPSblob->GetBufferPointer(), pPSblob->GetBufferSize(), NULL, &g_pLightPixelShader);
	pPSblob->Release();
	if (FAILED(hr))
	{
		return hr;
	}


	/*
		PIXEL SHADER ( for SOLID )
	*/
	pPSblob = nullptr;
	// FX 파일 중 PixelShader 로드
	hr = ComplieShaderFromFile(L"textureShader.fx", "PSSolid", "ps_4_0", &pPSblob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Load FX file FAILED", L"ERROR", MB_OK);
		return hr;
	}

	// Pixel Shader 생성
	hr = g_device->CreatePixelShader(pPSblob->GetBufferPointer(), pPSblob->GetBufferSize(), NULL, &g_pSolidPixelShader);
	pPSblob->Release();
	if (FAILED(hr))
	{
		return hr;
	}



	/*
		vertex list 로 vertex buffer 만들기
	*/
	SIMPLEVERTEX vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),	XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	};
	UINT numVertices = ARRAYSIZE(vertices);

	D3D11_BUFFER_DESC buffdesc;
	ZeroMemory(&buffdesc, sizeof(buffdesc));
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.ByteWidth = sizeof(SIMPLEVERTEX) * numVertices;
	buffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffdesc.CPUAccessFlags = 0;
	// 버텍스 버퍼 추가 정보
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertices;

	hr = g_device->CreateBuffer(&buffdesc, &initData, &g_pVertexBuffer);
	if (FAILED(hr))
		return hr;
	
	// 버텍스 버퍼 생성
	UINT stride = sizeof(SIMPLEVERTEX);
	UINT offset = 0;
	g_devcon->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	
	/*
		인덱스 버퍼 만들기
	*/
	WORD indices[] =
	{
		3, 1, 0,
		2, 1, 3,

		6, 4, 5,
		7, 4, 6,

		11, 9, 8,
		10, 9, 11,

		14, 12, 13,
		15, 12, 14,

		19, 17, 16,
		18, 17, 19,

		22, 20, 21,
		23, 20, 22
	};
	UINT numIndices = ARRAYSIZE(indices);

	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.ByteWidth = sizeof(WORD)* numIndices;
	buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffdesc.CPUAccessFlags = 0;
	initData.pSysMem = indices;
	// 인덱스 버퍼 생성
	hr = g_device->CreateBuffer(&buffdesc, &initData, &g_pIndexBuffer);
	if (FAILED(hr))
		return hr;
	// 인풋 어셈블러에 인덱스 버퍼 설정
	g_devcon->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// 프리미티브 방식은 Triangle List 로
	g_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	/*
		상수 버퍼 만들기 ( Constant buffer )
	*/
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.CPUAccessFlags = 0;

	// Never Changes
	buffdesc.ByteWidth = sizeof(CBNeverChanges);
	hr = g_device->CreateBuffer(&buffdesc, NULL, &g_pCBNeverChanges);
	if (FAILED(hr))
		return hr;

	// OnResize
	buffdesc.ByteWidth = sizeof(CBChangeOnResize);
	hr = g_device->CreateBuffer(&buffdesc, NULL, &g_pCBChangeOnResize);
	if (FAILED(hr))
		return hr;

	// Changes Every Frame
	buffdesc.ByteWidth = sizeof(CBChangesEveryFrame);
	hr = g_device->CreateBuffer(&buffdesc, NULL, &g_pCBChangesEveryFrame);
	if (FAILED(hr))
		return hr;



	/*
		외부에서 텍스쳐 파일 불러오기 ( .DDS )
	*/
	hr = D3DX11CreateShaderResourceViewFromFile(g_device, L"seafloor.dds", NULL, NULL, &g_pTextureRView, NULL);
	if (FAILED(hr))
		return hr;

	// 텍스쳐 샘플러 설정
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = g_device->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
	if (FAILED(hr))
		return hr;



	/*
		World, View, Projection 매트릭스 설정
	*/
	
	//WORLD MATRIX
	g_World			= XMMatrixIdentity();
	
	//VIEW MATRIX
	XMVECTOR Eye	= XMVectorSet(3.0f, 5.0f, -5.0f, 0.0f);
	XMVECTOR LookAt = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up		= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View			= XMMatrixLookAtLH(Eye, LookAt, Up);


	// view matrix 를 NeverChanges 상수 버퍼에 연결
	CBNEVERCHANGES cbNC;
	cbNC.mview = XMMatrixTranspose(g_View);
	g_devcon->UpdateSubresource(g_pCBNeverChanges, 0, NULL, &cbNC, 0, 0);

	
	//PROJECTION MATRIX
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, (FLOAT)width / (FLOAT)height, 0.01f, 100.0f);

	// projection matrix 를 Changes On Resize 상수 버퍼에 연결
	CBCHANGEONRESIZE cbCOR;
	cbCOR.mprojection = XMMatrixTranspose(g_Projection);
	g_devcon->UpdateSubresource(g_pCBChangeOnResize, 0, NULL, &cbCOR, 0, 0);


    return S_OK;
}

void	ReleaseDevice()
{
	if (g_devcon) g_devcon->ClearState();

	// texture interface 제거
	if (g_pSamplerLinear) g_pSamplerLinear->Release();
	if (g_pTextureRView) g_pTextureRView->Release();

	// constant buffer 들 제거
	if (g_pCBNeverChanges) g_pCBNeverChanges->Release();
	if (g_pCBChangeOnResize) g_pCBChangeOnResize->Release();
	if (g_pCBChangesEveryFrame) g_pCBChangesEveryFrame->Release();

	
	if (g_pLightPixelShader) g_pLightPixelShader->Release();

	if (g_pDepthStencilView) g_pDepthStencilView->Release();

	//if (g_pConstantBuffer) g_pConstantBuffer->Release();
	if (g_pIndexBuffer) g_pIndexBuffer->Release();

	if (g_pVertexBuffer) g_pVertexBuffer->Release();
	if (g_pVertexLayout) g_pVertexLayout->Release();
	if (g_pVertexShader) g_pVertexShader->Release();
	if (g_pSolidPixelShader) g_pSolidPixelShader->Release();

	if (g_rtBackbuffer) g_rtBackbuffer->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (g_devcon) g_devcon->Release();
	if (g_device) g_device->Release();
}



HRESULT ComplieShaderFromFile(WCHAR* wFilename, LPCSTR strEntry, LPCSTR strShaderMdl, ID3DBlob** ppblob)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob = nullptr;

	hr = D3DX11CompileFromFile(wFilename, NULL, NULL, 
							strEntry, strShaderMdl, dwShaderFlags, 0,
							NULL, ppblob, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}
		return hr;		
	}
	
	if ( pErrorBlob )
		pErrorBlob->Release();

	return S_OK;
}

void	Render()
{



	/*
		첫번째 박스 회전
	*/
	static float rot = 0.0f;
	rot += 0.0001f;
	g_World = XMMatrixRotationRollPitchYaw(0.0f, rot, 0.0f);
	g_World = g_World * XMMatrixTranslation(0.0f, 0.0f, 0.0f);

		
	/*
		조명 방향 과 색상 설정
	*/
	XMFLOAT4 LitDir[2] =
	{
		XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f),
		XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f),
	};
	XMFLOAT4 LitCol[2] =
	{
		XMFLOAT4(0.1f, 0.7f, 0.3f, 1.0f),
		XMFLOAT4(0.6f, 0.6f, 0.5f, 1.0f)
	};


	/*
		첫번째 조명
	*/
	XMVECTOR vAxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX mrot = XMMatrixRotationAxis(vAxis, -rot * 1.6f);
	XMVECTOR vdir = XMLoadFloat4(&LitDir[0]);
	vdir = XMVector3Transform(vdir, mrot);
	XMStoreFloat4(&LitDir[0], vdir);



	/*
		두번째 조명을 박스 주변을 회전하도록 설정
	*/
	XMMATRIX mRotate = XMMatrixRotationX(rot * 2.1f);
	XMVECTOR vLightDir = XMLoadFloat4(&LitDir[1]);
	vLightDir = XMVector3Transform(vLightDir, mRotate);
	XMStoreFloat4(&LitDir[1], vLightDir);



	// Modify the color
	g_vMeshColor.x = (sinf(rot * 1.0f) + 1.0f) * 0.5f;
	g_vMeshColor.y = (cosf(rot * 3.0f) + 1.0f) * 0.5f;
	g_vMeshColor.z = (sinf(rot * 5.0f) + 1.0f) * 0.5f;




	//red,green,blue,alpha
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	g_devcon->ClearRenderTargetView(g_rtBackbuffer, ClearColor);
	// depth buffer 청소 ( max depth 즉, 1.0f 로 설정)
	g_devcon->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	{

		/*
			Constant Buffer 연결
		*/
		CBCHANGESEVERYFRAME cb;		
		cb.mworld = XMMatrixTranspose(g_World);
		
		cb.litDir[0] = LitDir[0];
		cb.litDir[1] = LitDir[1];
		cb.litCol[0] = LitCol[0];
		cb.litCol[1] = LitCol[1];
		cb.colOutput = XMFLOAT4(0, 0, 0, 0);

		g_devcon->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);


		/*
			Box 그리기
		*/
		//VERTEX SHADER
		g_devcon->VSSetShader(g_pVertexShader, NULL, 0);
		g_devcon->VSSetConstantBuffers(0, 1, &g_pCBNeverChanges);
		g_devcon->VSSetConstantBuffers(1, 1, &g_pCBChangeOnResize);
		g_devcon->VSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
		//PIXEL SHADER
		g_devcon->PSSetShader(g_pLightPixelShader, NULL, 0);
		g_devcon->PSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
		g_devcon->PSSetShaderResources(0, 1, &g_pTextureRView);
		g_devcon->PSSetSamplers(0, 1, &g_pSamplerLinear);

		g_devcon->DrawIndexed(36, 0, 0);


		/*
			조명 그리기
		*/
		for (int l = 0; l < 2; l++)
		{
			XMMATRIX mlight = XMMatrixTranslationFromVector(5.0f * XMLoadFloat4(&LitDir[l]));
			XMMATRIX mlightScl = XMMatrixScaling(0.2f, 0.2f, 0.2f);

			mlight = mlightScl * mlight;

			cb.mworld = XMMatrixTranspose(mlight);
			cb.colOutput = LitCol[l];

			g_devcon->UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
			g_devcon->PSSetShader(g_pSolidPixelShader, NULL, 0);
			g_devcon->DrawIndexed(36, 0, 0);
		}


	}
	g_pSwapChain->Present(0, 0);
}