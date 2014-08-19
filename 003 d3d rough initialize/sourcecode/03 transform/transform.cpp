/*
	Transform
	- Local -> World -> View -> Projection

	Vertex Buffer

	Index Buffer

	과제 ]
		- 제자리 돌기
		- 궤도 따라 돌기
*/

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>

#include <D3Dcompiler.h>
#include <xnamath.h>


typedef struct Vertex
{
	XMFLOAT3 pos;	// 위치 정보
	XMFLOAT4 color;	// 색상(컬러) 정보

}VERTEX;

// 셰이더와 소스코드 연결위한 상수형 변수
typedef struct ConstantBuffer
{
	XMMATRIX mworld;
	XMMATRIX mview;
	XMMATRIX mprojection;

}CONSTANTBUFFER;



HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;

// 그려주기 위한 준비 작업
ID3D11Device*           g_device = NULL;
ID3D11DeviceContext*    g_devcon = NULL;
IDXGISwapChain*         g_swapchain = NULL;
ID3D11RenderTargetView* g_rtview = NULL;

// 실제로 그릴 데이터
ID3D11VertexShader*     g_vertexShader = NULL;
ID3D11InputLayout*      g_vertexLayout = NULL;
ID3D11PixelShader*      g_pixelShader = NULL;
ID3D11Buffer*           g_vertexBuffer = NULL;

ID3D11Buffer*           g_indexBuffer = NULL;
ID3D11Buffer*           g_constantBuffer = NULL;

XMMATRIX                g_World;
XMMATRIX                g_View;
XMMATRIX                g_Projection;



HRESULT		InitDevice();
void		ReleaseDevice();

// 셰이더를 작동할 함수
HRESULT		CompileShaderFromFile(WCHAR* wFilename, LPCSTR strEntry,
								LPCSTR strShaderMdl, ID3DBlob** ppblob);
void		RenderTriangle();

/*
	윈도 메시지 프로시져
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	윈 메인
*/
int WINAPI wWinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	wchar_t appname[50] = L"001 Basics";

	// 윈도우 구조체 채우기
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_WINLOGO);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = appname;
	wcex.hIconSm = wcex.hIcon;
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// 윈도우 생성
	g_hInst = hInstance;
	RECT rc = { 0, 0, 1024, 768 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow(appname, appname, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, 
		NULL, NULL, hInstance, NULL);

	// 윈도 출력
	ShowWindow(g_hWnd, nCmdShow);

	// d3d device 생성
	InitDevice();

	// 기본 메시지 루프
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //MainLoop
		{
			// 업데이트..

			/*
				박스 애니메이션 해보기
			*/
			static float rot = 0.0f;
			rot += 0.0001f;			
			g_World = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, rot);
			g_World = g_World * XMMatrixTranslation(0.0f, 0.0f, 0.0f);


			// 백버퍼 지우기
			float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
			g_devcon->ClearRenderTargetView(g_rtview, ClearColor);
			
			
			// 렌더링..
			RenderTriangle();

			
			// 더블 버퍼링
			g_swapchain->Present(0, 0);
		}
	}//while

	// D3d device 들 해제
	ReleaseDevice();

	return (int)msg.wParam;
}


HRESULT		InitDevice()
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(g_hWnd, &rc);
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
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	// 백버퍼에 대한 설정
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
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

	D3D_DRIVER_TYPE		driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL	featureLevel = D3D_FEATURE_LEVEL_11_0;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; 
														driverTypeIndex++)
	{
		driverType = driverTypes[driverTypeIndex];

		// Device , Device Context , SwapChain 을 동시에 생성한다.
		hr = D3D11CreateDeviceAndSwapChain(NULL, driverType, NULL, 
							createDeviceFlags, featureLevels, numFeatureLevels,
							D3D11_SDK_VERSION, &sd, 
							&g_swapchain, &g_device, &featureLevel, &g_devcon);

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	/*
		렌더 타겟 뷰 생성
	*/
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = g_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), 
											(LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return hr;

	hr = g_device->CreateRenderTargetView(pBackBuffer, NULL, &g_rtview);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	// device context 에 렌더타겟 연결
	g_devcon->OMSetRenderTargets(1, &g_rtview, NULL);


	// Viewport 세팅
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	g_devcon->RSSetViewports(1, &vp);


	/*
		VERTEX SHADER
	*/
	ID3DBlob* pVSblob = nullptr;
	// FX 파일 중 VertexShader 로드
	hr = CompileShaderFromFile(L"transformShader.fx", "VS", "vs_4_0", &pVSblob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Load FX file FAILED", L"ERROR", MB_OK);
		return hr;
	}
	// Vertex Shader 생성
	hr = g_device->CreateVertexShader(
								pVSblob->GetBufferPointer(),
								pVSblob->GetBufferSize(), NULL, 
								&g_vertexShader);
	/*
		VERTEX LAYOUT
	*/
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	hr = g_device->CreateInputLayout(layout, numElements, 
								pVSblob->GetBufferPointer(), 
								pVSblob->GetBufferSize(), 
								&g_vertexLayout);
	pVSblob->Release();

	// VertexLayout 을 device Context 에 설정.
	g_devcon->IASetInputLayout(g_vertexLayout);
	

	/*
		PIXEL SHADER
	*/
	ID3DBlob* pPSblob = nullptr;
	// FX 파일 중 PixelShader 로드
	hr = CompileShaderFromFile(L"transformShader.fx", "PS", "ps_4_0", &pPSblob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Load FX file FAILED", L"ERROR", MB_OK);
		return hr;
	}
	// Pixel Shader 생성
	hr = g_device->CreatePixelShader(
								pPSblob->GetBufferPointer(),
								pPSblob->GetBufferSize(), NULL,
								&g_pixelShader);
	pPSblob->Release();

	/*
		VERTEX BUFFER 만들기
	*/
	VERTEX vertices[] =
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
	buffdesc.ByteWidth = sizeof(VERTEX)* numVertices;
	buffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffdesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertices;
	//버텍스 버퍼 생성
	hr = g_device->CreateBuffer(&buffdesc, &initData, &g_vertexBuffer);
	
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	//버텍스 버퍼를 IA 에 연결
	g_devcon->IASetVertexBuffers(0, 1, &g_vertexBuffer, &stride, &offset);



	/*
		INDEX BUFFER 만들기
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
	buffdesc.ByteWidth = sizeof(WORD)* numIndices;
	buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffdesc.CPUAccessFlags = 0;	
	initData.pSysMem = indices;

	//인덱스 버퍼 생성
	hr = g_device->CreateBuffer(&buffdesc, &initData, &g_indexBuffer);

	//IA 에다가 인덱스 버퍼 연결
	g_devcon->IASetIndexBuffer(g_indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//TRIANGLELIST 방식으로 그리기
	g_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	/*
		상수 버퍼 만들기 ( Constant buffer )
	*/
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.ByteWidth = sizeof(CONSTANTBUFFER);
	buffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.CPUAccessFlags = 0;

	// 상수 버퍼 생성
	hr = g_device->CreateBuffer(&buffdesc, NULL, &g_constantBuffer);
	if (FAILED(hr))
		return hr;


	/*
		World, View, Projection 
	*/
	g_World = XMMatrixIdentity();

	XMVECTOR eye	= XMVectorSet(0.0f, 2.0f, -10.0f, 0.0f);
	XMVECTOR lookat = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR up		= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(eye, lookat, up);

	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, (FLOAT)width / (FLOAT)height, 0.01f, 100.0f);


	return S_OK;
}

void ReleaseDevice()
{
	if (g_devcon)			g_devcon->ClearState();

	if (g_constantBuffer)	g_constantBuffer->Release();
	if (g_indexBuffer)		g_indexBuffer->Release();

	if (g_vertexBuffer)		g_vertexBuffer->Release();
	if (g_vertexLayout)		g_vertexLayout->Release();
	if (g_vertexShader)		g_vertexShader->Release();
	if (g_pixelShader)		g_pixelShader->Release();

	if (g_rtview)			g_rtview->Release();
	if (g_swapchain)		g_swapchain->Release();
	if (g_devcon)			g_devcon->Release();
	if (g_device)			g_device->Release();
}


/*
	외부 셰이더 파일을 불러서 Device에 연결하기
*/
HRESULT CompileShaderFromFile(WCHAR* wFilename, LPCSTR strEntry,
							LPCSTR strShaderMdl, ID3DBlob** ppblob)
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

	if (pErrorBlob)
		pErrorBlob->Release();

	return hr;
}


void RenderTriangle()
{
	CONSTANTBUFFER cb;
	cb.mworld = XMMatrixTranspose(g_World);
	cb.mview = XMMatrixTranspose(g_View);
	cb.mprojection = XMMatrixTranspose(g_Projection);

	// 셰이더와 소스를 연결
	g_devcon->UpdateSubresource(g_constantBuffer, 0, NULL, &cb, 0, 0);
	

	// 박스 그리기
	g_devcon->VSSetShader(g_vertexShader, NULL, 0);
	g_devcon->VSSetConstantBuffers(0, 1, &g_constantBuffer);
	
	g_devcon->PSSetShader(g_pixelShader, NULL, 0);

	// 인덱스 버퍼로 그리기
	g_devcon->DrawIndexed(36, 0, 0);
}