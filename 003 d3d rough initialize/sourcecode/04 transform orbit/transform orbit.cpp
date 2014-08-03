/*
	기본 박스 의 움직임에 , 궤도 회전을 하는 박스를 추가한다.

	두 박스의 가려짐 판단을 위한 Depth Stencil Buffer 를 추가한다.
*/
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>

#include <D3Dcompiler.h>
#include <xnamath.h>



typedef struct SimpleVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;

}SIMPLEVERTEX;

typedef struct ConstantBuffer
{
	XMMATRIX mworld;
	XMMATRIX mview;
	XMMATRIX mprojection;

}CONSTANTBUFFER;


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
ID3D11PixelShader*      g_pPixelShader = NULL;
ID3D11InputLayout*      g_pVertexLayout = NULL;
ID3D11Buffer*           g_pVertexBuffer = NULL;

ID3D11Buffer*           g_pIndexBuffer = NULL;
ID3D11Buffer*           g_pConstantBuffer = NULL;
XMMATRIX                g_ParentMat;
XMMATRIX                g_View;
XMMATRIX                g_Projection;

// depth stencil 버퍼 추가
ID3D11Texture2D*        g_pDepthStencil = NULL;
ID3D11DepthStencilView* g_pDepthStencilView = NULL;
// 두번째 박스의 월드 매트릭스 추가
XMMATRIX				g_ChildMat;

XMFLOAT3				g_pos;



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

	WCHAR classname[] = L"004 Transform Orbit & Depth Stencil buffer";

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
    RECT rc = { 0, 0, 800, 600 };
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

			/*
				첫번째 박스 회전
			*/
			static float rot = 0.0f;
			rot += 0.0001f;			
			g_ParentMat = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, rot);
			g_ParentMat = g_ParentMat * XMMatrixTranslation(g_pos.x, g_pos.y, 0.0f);


			/*
				두번째 박스 회전 - 궤도 회전

				parentMat	= Scale * Rotation * Translate

				childMat	= local(S R T) * parentMat;
			*/
			XMMATRIX mscale = XMMatrixScaling(0.5f, 0.5f, 0.5f);
			XMMATRIX mspin = XMMatrixRotationRollPitchYaw(-rot*20.0f, 0.0f, 0.0f);
			XMMATRIX mtranslate = XMMatrixTranslation(3.0f, 0.0f, 0.0f);
			XMMATRIX morbit = XMMatrixRotationY(-rot * 3.0f);
			
			//Child		=	Local ( S * R * T )				* Parent
			g_ChildMat = ( mscale * mspin * mtranslate * morbit )* g_ParentMat;


			//red,green,blue,alpha
			float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; 
			g_devcon->ClearRenderTargetView(g_rtBackbuffer, ClearColor);
			// depth buffer 청소 ( max depth 즉, 1.0f 로 설정)
			g_devcon->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
			{
				Render();
			}
			g_pSwapChain->Present(0, 0);
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
			g_pos.y += 1.0f;
		}
		else if (wParam == VK_DOWN)
		{
			g_pos.y -= 1.0f;
		}

		if (wParam == VK_LEFT)
		{
			g_pos.x -= 1.0f;
		}
		else if (wParam == VK_RIGHT)
		{
			g_pos.x += 1.0f;
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
	hr = g_device->CreateTexture2D(&depthDesc, NULL, &g_pDepthStencil);
	if (FAILED(hr))
		return hr;


	// depth stencil view 생성
	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	ZeroMemory(&DSVDesc, sizeof(DSVDesc));
	DSVDesc.Format = depthDesc.Format;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Texture2D.MipSlice = 0;

	hr = g_device->CreateDepthStencilView(g_pDepthStencil, &DSVDesc, &g_pDepthStencilView);
	if (FAILED(hr))
		return hr;
	
	g_devcon->OMSetRenderTargets(1, &g_rtBackbuffer, g_pDepthStencilView);
	//g_devcon->OMSetRenderTargets(1, &g_rtBackbuffer, NULL);


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
	hr = ComplieShaderFromFile(L"orbitShader.fx", "VS", "vs_4_0", &pVSblob);
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
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
		PIXEL SHADER
	*/

	ID3DBlob* pPSblob = nullptr;
	// FX 파일 중 PixelShader 로드
	hr = ComplieShaderFromFile(L"orbitShader.fx", "PS", "ps_4_0", &pPSblob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Load FX file FAILED", L"ERROR", MB_OK);
		return hr;
	}

	// Pixel Shader 생성
	hr = g_device->CreatePixelShader(pPSblob->GetBufferPointer(), pPSblob->GetBufferSize(), NULL, &g_pPixelShader);
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
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
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

		0, 5, 4,
		1, 5, 0,

		3, 4, 7,
		0, 4, 3,

		1, 6, 5,
		2, 6, 1,

		2, 7, 6,
		3, 7, 2,

		6, 4, 5,
		7, 4, 6,
	};
	UINT numIndices = ARRAYSIZE(indices);

	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.ByteWidth = sizeof(WORD)* numIndices;        // 36 vertices needed for 12 triangles in a triangle list
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
	buffdesc.ByteWidth = sizeof(ConstantBuffer);
	buffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.CPUAccessFlags = 0;

	// 상수 버퍼 생성
	hr = g_device->CreateBuffer(&buffdesc, NULL, &g_pConstantBuffer);
	if (FAILED(hr))
		return hr;


	/*
		World, View, Projection 매트릭스 초기화
	*/
	
	//WORLD MATRIX
	g_ParentMat = XMMatrixIdentity();

	//새로 추가한 cube 의 월드 행렬을 초기화
	g_ChildMat = XMMatrixIdentity();

	//VIEW MATRIX
	XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -10.0f, 0.0f);
	XMVECTOR LookAt = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, LookAt, Up);

	//PROJECTION MATRIX
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, (FLOAT)width / (FLOAT)height, 0.01f, 100.0f);

    return S_OK;
}

void	ReleaseDevice()
{
	if (g_devcon) g_devcon->ClearState();

	// 새로 추가한 depth stencil 버퍼들도 삭제
	if (g_pDepthStencil) g_pDepthStencil->Release();
	if (g_pDepthStencilView) g_pDepthStencilView->Release();

	if (g_pConstantBuffer) g_pConstantBuffer->Release();
	if (g_pIndexBuffer) g_pIndexBuffer->Release();

	if (g_pVertexBuffer) g_pVertexBuffer->Release();
	if (g_pVertexLayout) g_pVertexLayout->Release();
	if (g_pVertexShader) g_pVertexShader->Release();
	if (g_pPixelShader) g_pPixelShader->Release();

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
		Constant Buffer 연결
	*/
	ConstantBuffer cb;
	cb.mworld = XMMatrixTranspose(g_ParentMat);
	cb.mview = XMMatrixTranspose(g_View);
	cb.mprojection = XMMatrixTranspose(g_Projection);
	g_devcon->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

	/*
		Box 그리기
	*/	
	g_devcon->VSSetShader(g_pVertexShader, NULL, 0);
	// 상수 버퍼 연결
	g_devcon->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_devcon->PSSetShader(g_pPixelShader, NULL, 0);
	// 인덱스 버퍼로 그리기
	g_devcon->DrawIndexed(36, 0, 0);


	/*
		두번째 박스용 Constant Buffer 연결
	*/
	ConstantBuffer cb2;
	cb2.mworld = XMMatrixTranspose(g_ChildMat);
	cb2.mview = XMMatrixTranspose(g_View);
	cb2.mprojection = XMMatrixTranspose(g_Projection);
	g_devcon->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb2, 0, 0);

	// 인덱스 버퍼로 그리기
	g_devcon->DrawIndexed(36, 0, 0);

}