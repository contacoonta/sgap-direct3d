/*
	d3d device initialize

	- Device
		: D3d �� �Ѱ��ϴ� �������̽�
	- Device Context
		: �׷��ֱ����� �������̽�
	- Swap Chain
		: Page Flipping , ������۸�
	- Render Target View
		: �׸� ����
*/

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>

HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;

ID3D11Device*           g_device = NULL;
ID3D11DeviceContext*    g_devcon = NULL;
IDXGISwapChain*         g_swapchain = NULL;
ID3D11RenderTargetView* g_rtview = NULL;



HRESULT		InitDevice();
void		ReleaseDevice();


/*
	���� �޽��� ���ν���
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
	�� ����
*/
int WINAPI wWinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	wchar_t appname[50] = L"001 Basics";

	// ������ ����ü ä���
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

	// ������ ����
	g_hInst = hInstance;
	RECT rc = { 0, 0, 1024, 768 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow(appname, appname, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, 
		NULL, NULL, hInstance, NULL);

	// ���� ���
	ShowWindow(g_hWnd, nCmdShow);

	// d3d device ����
	InitDevice();

	// �⺻ �޽��� ����
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
			// ������Ʈ..

			// ����� �����
			float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
			g_devcon->ClearRenderTargetView(g_rtview, ClearColor);
			
			
			// ������..

			
			// ���� ���۸�
			g_swapchain->Present(0, 0);
		}
	}//while

	// D3d device �� ����
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

	// ����ۿ� ���� ����
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

		// Device , Device Context , SwapChain �� ���ÿ� �����Ѵ�.
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
		���� Ÿ�� �� ����
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

	// device context �� ����Ÿ�� ����
	g_devcon->OMSetRenderTargets(1, &g_rtview, NULL);


	// Viewport ����
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	g_devcon->RSSetViewports(1, &vp);

	return S_OK;
}

void ReleaseDevice()
{
	if (g_devcon)		g_devcon->ClearState();

	if (g_rtview)		g_rtview->Release();
	if (g_swapchain)	g_swapchain->Release();
	if (g_devcon)		g_devcon->Release();
	if (g_device)		g_device->Release();
}