/*
	1. ���ؽ� ���̾ƿ��� , POSITIOIN �� NORMAL �� �����Ѵ�.

	2. LIGHT �� �����Ͽ� , NORMAL �� LIGHT �� �������� ǥ���� ��⸦ �����Ѵ�.


	���� ]
		1. �ι�° �ڽ� ���� ����� �ް� ���� ó�� �ǰ��ϱ�.
		
		2. ���� �ð��� ���� �¾�迡�� �¾��� ������ �������� ����.

	Ȯ�� ]]

		3. �� ���� ���� ( �Ķ� ���� ����.)

*/

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>

#include <D3Dcompiler.h>
#include <xnamath.h>



typedef struct SimpleVertex
{
	XMFLOAT3 pos;
	XMFLOAT3 nor;				// ���ؽ��� �븻 ����

}SIMPLEVERTEX, *LPSIMPLEVERTEX;

typedef struct ConstantBuffer
{
	XMMATRIX mworld;
	XMMATRIX mview;
	XMMATRIX mprojection;

	XMFLOAT4 litDir;		// ������ ����
	XMFLOAT4 litCol;		// ������ ��

	XMFLOAT4 colOutput;		// ���� �ȼ� ����

}CONSTANTBUFFER;



HINSTANCE   g_hInst = NULL;
HWND        g_hWnd = NULL;

D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*           g_device = NULL;
ID3D11DeviceContext*    g_devcon = NULL;
IDXGISwapChain*         g_swapChain = NULL;
ID3D11RenderTargetView* g_rtView = NULL;

ID3D11VertexShader*     g_pVertexShader = NULL;
ID3D11PixelShader*      g_pSolidPixelShader = NULL;
ID3D11InputLayout*      g_pVertexLayout = NULL;
ID3D11Buffer*           g_pVertexBuffer = NULL;

ID3D11Buffer*           g_pIndexBuffer = NULL;
ID3D11Buffer*           g_pConstantBuffer = NULL;
XMMATRIX                g_ParentMat;
XMMATRIX                g_View;
XMMATRIX                g_Projection;

// depth stencil ���� �߰�
ID3D11DepthStencilView* g_pDepthStencilView = NULL;

// �ι�° �ڽ��� ���� ��Ʈ���� �߰�
XMMATRIX				g_ChildMat;

// �ȼ� ���̴� �߰� ( ���� ���� )
ID3D11PixelShader*      g_pLightPixelShader = NULL;



HRESULT InitDevice();
HRESULT CompileShaderFromFile(WCHAR* wFilename, LPCSTR strEntry, 
							LPCSTR strShaderMdl, ID3DBlob** ppblob);
void	ReleaseDevice();
void	Render();

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// Register class
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
	wcex.lpszClassName = L"001 Basics";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_WINLOGO);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	g_hInst = hInstance;
	RECT rc = { 0, 0, 1024, 768 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow(L"001 Basics", L"Direct3D 11 Initialize", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);

	if (!g_hWnd)
		return E_FAIL;

	ShowWindow(g_hWnd, nCmdShow);

	
	InitDevice();



	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

			Render();			

		}
	}//while()

	ReleaseDevice();

	return (int)msg.wParam;
}

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


HRESULT InitDevice()
{

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
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	// ����ü�� ����
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


	HRESULT hr = S_FALSE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, 
			featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &g_swapChain, &g_device, &g_featureLevel, &g_devcon);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return hr;

	hr = g_device->CreateRenderTargetView(pBackBuffer, NULL, &g_rtView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	/*
		DEPTH STENCIL BUFFER
	*/
	// depth stencil �ؽ��� ����
	D3D11_TEXTURE2D_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.Width = width;
	depthDesc.Height = height;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //32bit , depth 24 bit, stencil 8 bit
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;

	// Device �� depth stencil �� �ؽ��� ����
	ID3D11Texture2D* pDepthStencil = NULL;
	hr = g_device->CreateTexture2D(&depthDesc, NULL, &pDepthStencil);
	if (FAILED(hr))
		return hr;


	// depth stencil view ����
	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	ZeroMemory(&DSVDesc, sizeof(DSVDesc));
	DSVDesc.Format = depthDesc.Format;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Texture2D.MipSlice = 0;

	hr = g_device->CreateDepthStencilView(pDepthStencil, &DSVDesc,
											&g_pDepthStencilView);
	pDepthStencil->Release();
	
	if (FAILED(hr))
		return hr;
	
	g_devcon->OMSetRenderTargets(1, &g_rtView, g_pDepthStencilView);


	// Setup the viewport
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
	// FX ���� �� VertexShader �ε�
	hr = CompileShaderFromFile(L"lightShader.fx", "VERTEXSHADER", "vs_4_0", &pVSblob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Load FX file FAILED", L"ERROR", MB_OK);
		return hr;
	}

	// Vertex Shader ����
	hr = g_device->CreateVertexShader(pVSblob->GetBufferPointer(), 
								pVSblob->GetBufferSize(), NULL, &g_pVertexShader);
	if (FAILED(hr))
	{
		pVSblob->Release();
		return hr;
	}


	/*
		VERTEX LAYOUT
	*/

	// Shader Layout ����
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(layout);

	// Layout ����ü ������ �������� VertexLayout �� ����.
	hr = g_device->CreateInputLayout(layout, numElements, 
		pVSblob->GetBufferPointer(), pVSblob->GetBufferSize(), &g_pVertexLayout);
	pVSblob->Release();
	if (FAILED(hr))
		return hr;

	// VertexLayout �� device Context �� ����.
	g_devcon->IASetInputLayout(g_pVertexLayout);



	/*
		PIXEL SHADER ( for LIGHT )
	*/

	ID3DBlob* pPSblob = nullptr;
	// FX ���� �� PixelShader �ε�
	hr = CompileShaderFromFile(L"lightShader.fx", "PSLight", "ps_4_0", &pPSblob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Load FX file FAILED", L"ERROR", MB_OK);
		return hr;
	}

	// Pixel Shader ����
	hr = g_device->CreatePixelShader(pPSblob->GetBufferPointer(), 
								pPSblob->GetBufferSize(), NULL, &g_pLightPixelShader);
	pPSblob->Release();
	if (FAILED(hr))
	{
		return hr;
	}


	/*
		PIXEL SHADER ( for SOLID )
	*/

	pPSblob = nullptr;
	// FX ���� �� PixelShader �ε�
	hr = CompileShaderFromFile(L"lightShader.fx", "PSSolid", "ps_4_0", &pPSblob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Load FX file FAILED", L"ERROR", MB_OK);
		return hr;
	}

	// Pixel Shader ����
	hr = g_device->CreatePixelShader(pPSblob->GetBufferPointer(), 
								pPSblob->GetBufferSize(), NULL, &g_pSolidPixelShader);
	pPSblob->Release();
	if (FAILED(hr))
	{
		return hr;
	}


	/*
		vertex list �� vertex buffer �����
	*/
	SIMPLEVERTEX vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },

		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
	};
	UINT numVertices = ARRAYSIZE(vertices);

	/*
		���ؽ� ���� ����.
	*/
	D3D11_BUFFER_DESC buffdesc;
	ZeroMemory(&buffdesc, sizeof(buffdesc));
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.ByteWidth = sizeof(SIMPLEVERTEX)* numVertices;
	buffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffdesc.CPUAccessFlags = 0;
	//
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertices;

	hr = g_device->CreateBuffer(&buffdesc, &initData, &g_pVertexBuffer);
	if (FAILED(hr))
		return hr;

	// devcon �� ���ؽ� ���۸� ����
	UINT stride = sizeof(SIMPLEVERTEX);
	UINT offset = 0;
	g_devcon->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);


	/*
		�ε��� ���� �����
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
	// 12�� �ﰢ�� �׸����� , 36�� �ε����� �ʿ��ϴ�.
	buffdesc.ByteWidth = sizeof(WORD)* numIndices;        
	buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffdesc.CPUAccessFlags = 0;
	initData.pSysMem = indices;
	// �ε��� ���� ����
	hr = g_device->CreateBuffer(&buffdesc, &initData, &g_pIndexBuffer);
	if (FAILED(hr))
		return hr;
	// ��ǲ ������� �ε��� ���� ����
	g_devcon->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	
	// 
	g_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	/*
		��� ���� ����� ( Constant buffer )
	*/
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.ByteWidth = sizeof(ConstantBuffer);
	buffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.CPUAccessFlags = 0;

	// ��� ���� ����
	hr = g_device->CreateBuffer(&buffdesc, NULL, &g_pConstantBuffer);
	if (FAILED(hr))
		return hr;

	/*
		World, View, Projection ��Ʈ���� �ʱ�ȭ
	*/
	
	//WORLD MATRIX
	g_ParentMat = XMMatrixIdentity();

	//CHILD MATRIX
	g_ChildMat = XMMatrixIdentity();


	//VIEW MATRIX
	XMVECTOR Eye = XMVectorSet(3.0f, 8.0f, -6.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);

	//PROJECTION MATRIX
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 
											(FLOAT)width / (FLOAT)height, 
											0.01f, 100.0f);


}

void	ReleaseDevice()
{
	if (g_devcon) g_devcon->ClearState();

	// light pixel shader ����
	if (g_pLightPixelShader) g_pLightPixelShader->Release();

	if (g_pConstantBuffer) g_pConstantBuffer->Release();
	if (g_pIndexBuffer) g_pIndexBuffer->Release();

	if (g_pVertexBuffer) g_pVertexBuffer->Release();
	if (g_pVertexLayout) g_pVertexLayout->Release();
	if (g_pVertexShader) g_pVertexShader->Release();
	if (g_pSolidPixelShader) g_pSolidPixelShader->Release();

	if (g_pDepthStencilView) g_pDepthStencilView->Release();

	if (g_rtView) g_rtView->Release();
	if (g_swapChain) g_swapChain->Release();
	if (g_devcon) g_devcon->Release();
	if (g_device) g_device->Release();
}



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

	if ( pErrorBlob )
		pErrorBlob->Release();

	return S_OK;
}

void	Render()
{

	/*
		�ڽ� �ִϸ��̼� �غ���
	*/
	static float rot = 0.0f;
	rot += 0.0001f;
	g_ParentMat = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, rot);
	g_ParentMat = g_ParentMat * XMMatrixTranslation(2.0f, 0.0f, 0.0f);

	//XMVECTOR vAxis = XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f);
	//XMMATRIX morbit = XMMatrixRotationAxis(vAxis, rot);
	XMMATRIX mscale = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	XMMATRIX mspin = XMMatrixRotationRollPitchYaw(-rot*5.0f, -rot, 0.0f);
	XMMATRIX mtranslate = XMMatrixTranslation(0.0f, 3.0f, 0.0f);
	//LOCAL( SCALE * ROTATION * TRANSLATE) * WORLD ( ORBIT )
	g_ChildMat = (mscale * mspin * mtranslate) * g_ParentMat;


	/*
		���� ���� �� ���� ����
	*/
	XMFLOAT4 LitDir = XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f);
	XMFLOAT4 LitCol = XMFLOAT4(0.8f, 1.2f, 0.3f, 1.0f);


	XMVECTOR vAxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX mrot = XMMatrixRotationAxis(vAxis, -rot * 1.6f);
	XMVECTOR vdir = XMLoadFloat4(&LitDir);
	vdir = XMVector3Transform(vdir, mrot);
	XMStoreFloat4(&LitDir, vdir);



	//red,green,blue,alpha
	float ClearColor[4] = { 0.0f, 0.2f, 1.0f, 1.0f };
	g_devcon->ClearRenderTargetView(g_rtView, ClearColor);
	g_devcon->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	{

		/*
			Constant Buffer ����
		*/
		CONSTANTBUFFER cb;
		cb.mworld = XMMatrixTranspose(g_ParentMat);
		cb.mview = XMMatrixTranspose(g_View);
		cb.mprojection = XMMatrixTranspose(g_Projection);

		cb.litDir = LitDir;
		cb.litCol = LitCol;
		cb.colOutput = XMFLOAT4(0, 0, 0, 0);

		g_devcon->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

		/*
			Box �׸���
		*/
		g_devcon->VSSetShader(g_pVertexShader, NULL, 0);
		g_devcon->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);

		g_devcon->PSSetShader(g_pLightPixelShader, NULL, 0);
		g_devcon->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
		
		g_devcon->DrawIndexed(36, 0, 0);


		/*
			�ι�° �ڽ� �׸���
		*/
		CONSTANTBUFFER cb2;
		cb2.mworld = XMMatrixTranspose(g_ChildMat);
		cb2.mview = XMMatrixTranspose(g_View);
		cb2.mprojection = XMMatrixTranspose(g_Projection);

		cb2.litDir = LitDir;
		cb2.litCol = LitCol;
		cb2.colOutput = XMFLOAT4(0, 0, 0, 0);

		g_devcon->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb2, 0, 0);
		
		g_devcon->PSSetShader(g_pLightPixelShader, NULL, 0);
		g_devcon->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);

		g_devcon->DrawIndexed(36, 0, 0);



		/*
			���� �׸���
		*/
		XMMATRIX mlight = XMMatrixTranslationFromVector(5.0f * XMLoadFloat4(&LitDir));
		XMMATRIX mlightScl = XMMatrixScaling(0.2f, 0.2f, 0.2f);

		mlight = mlightScl * mlight;

		cb.mworld = XMMatrixTranspose(mlight);
		cb.colOutput = LitCol;

		g_devcon->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);
		g_devcon->PSSetShader(g_pSolidPixelShader, NULL, 0);
		g_devcon->DrawIndexed(36, 0, 0);
				
	}
	g_swapChain->Present(0, 0);

}