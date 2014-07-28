#include "graphicD3d.h"


SGAP_BEGIN

/*
*/
bool graphicD3d::CreateGraphicD3d(graphicD3d** ppgd3d, int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	if (*ppgd3d != nullptr)
		return false;

	*ppgd3d = new graphicD3d;
	return (*ppgd3d)->Initialize(screenWidth, screenHeight, vsync, hwnd, fullscreen, screenDepth, screenNear);
}

void graphicD3d::DestroyGraphicD3d(graphicD3d** ppgd3d)
{
	if (*ppgd3d)
	{
		(*ppgd3d)->Release();

		delete *ppgd3d;
		*ppgd3d = nullptr;
	}
}

/*
*/
graphicD3d::graphicD3d()
{
}

graphicD3d::graphicD3d(const graphicD3d&)
{
}

graphicD3d::~graphicD3d()
{
}

/*
*/
bool graphicD3d::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	HRESULT hr;

	IDXGIFactory*			factory;
	IDXGIAdapter*			adapter;
	IDXGIOutput*			adapterOutput;
	UINT					nummodes, numerator, denominator;
	DXGI_MODE_DESC*			displayModelist;
	DXGI_ADAPTER_DESC		adapterDesc;

	DXGI_SWAP_CHAIN_DESC			swapchainDesc;
	D3D11_TEXTURE2D_DESC			depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC		depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC	depthStencilViewDesc;	
	D3D11_RASTERIZER_DESC			rasterDesc;
	D3D11_VIEWPORT					viewport;


	// ��������ȭ ���� ( true / false )
	m_vsync = vsync;

	//DirectX Graphic Infrastructure �������̽� ���丮 �����.
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(hr))
		return false;
	
	//ù��° �׷��� ī�� ����� �����.
	hr = factory->EnumAdapters(0, &adapter);
	if (FAILED(hr))
		return false;

	//ù��° ������� ����� �����.
	hr = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(hr))
		return false;

	//DXGI_FORMAT_R8G8B8A8_UNORM ����� ��� ���信 �´� ����� ���� ���ϱ�.
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &nummodes, NULL);
	if (FAILED(hr))
		return false;

	//��� ������ ��� ����Ϳ� �׷���ī�� ������ ������ ����Ʈ ����.
	displayModelist = new DXGI_MODE_DESC[nummodes];
	if (!displayModelist)
		return false;

	//displayModelist �� ���� ä���ֱ�.
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &nummodes, displayModelist);
	if (FAILED(hr))
		return false;

	//�׷��� ī��� ������� ȭ�� �ʺ�/���� ������ ã�� �Ϸ�.
	//������� refresh rate �� �и� �� ���� ���� ����.
	for (int i = 0; i < nummodes; i++)
	{
		if (displayModelist[i].Width == (UINT)screenWidth &&
			displayModelist[i].Height == (UINT)screenHeight)
		{
			numerator = displayModelist[i].RefreshRate.Numerator;
			denominator = displayModelist[i].RefreshRate.Denominator;
		}
	}


	/*
	*/


	//�׷��� ī�� ���� ��������.
	hr = adapter->GetDesc(&adapterDesc);
	if (FAILED(hr))
		return false;

	//�׷��� ī�� �޸� ũ�� ��������. ( MegaBytes )
	m_videoMemory = UINT(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	
	//�׷��� ī�� �̸� ��������.
	lstrcpynW(m_videoDescription, adapterDesc.Description, 128);


	/*
	*/
	// ���ϴ� ���� ��ȸ ������, �ش� ���ҽ��� ����
	delete [] displayModelist;
	displayModelist = nullptr;

	//����� ��� ����� ����
	adapterOutput->Release();
	adapterOutput = nullptr;

	//�׷���ī�� ����� ����
	adapter->Release();
	adapter = nullptr;

	//���丮 ����
	factory->Release();
	factory = nullptr;
	

	/*
		����ü�� ����
	*/

    ZeroMemory( &swapchainDesc, sizeof( swapchainDesc ) );
	//����� ������ 1���� ����.
    swapchainDesc.BufferCount = 1;
	swapchainDesc.BufferDesc.Width = screenWidth;
	swapchainDesc.BufferDesc.Height = screenHeight;
    swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	if (m_vsync) {
		//vsync ��ġ�� ���缭 page flip �� �Ͼ��.
		swapchainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapchainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else {
		//vsync �� ������� �ִ��� ���� �׸��� �Ѵ�.
		swapchainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	// ����۸� ���� Ÿ�� �뵵�� ��� ����.
    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchainDesc.OutputWindow = hwnd;
	//��Ƽ���ø� ������ ����.
    swapchainDesc.SampleDesc.Count = 1;
    swapchainDesc.SampleDesc.Quality = 0;
	//��üȭ�� ��� â ��� ����.
	if (fullscreen) {
		swapchainDesc.Windowed = FALSE;
	}
	else {
		swapchainDesc.Windowed = TRUE;
	}
	//��ĵ���� ���� �� ��뿩�θ� ���� (unspecified) ���� ����.
	swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	//��� �� ����� ���� �������� ����.
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//�߰� �ɼ� �÷� ��� ����.
	swapchainDesc.Flags = 0;


	/*
		����̽� ����
	*/
    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//�׷��� ī�尡 dx11 �����Ǹ�  HARDWARE,
	//�����ϸ� REFERENCE �� ó���ȴ�. ( �ӵ��� HARDWARE ��� 1/1000 ���� ���� )
    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

	//directx ���� ����
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        m_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( NULL, m_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &swapchainDesc, &m_swapChain, &m_device, &m_featureLevel, &m_deviceContext );
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return false;



	/*
		����Ÿ�ٺ� (render target view) �����
	*/
    //����� �����͸� ��������.
    ID3D11Texture2D* pBackBuffer = NULL;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if( FAILED( hr ) )
        return false;

	//����� �����ͷ� ����Ÿ�ٺ� ����.
    hr = m_device->CreateRenderTargetView( pBackBuffer, NULL, &m_renderTargetView );
    if( FAILED( hr ) )
        return false;

	//����Ÿ�� �����Ǹ� ����� ������ �ʿ�����Ƿ� ����.
    pBackBuffer->Release();
	pBackBuffer = nullptr;



	/*
		���̹��� (depth buffer) ����� (�ɼ�)
	*/
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//���̹��� 2D�ؽ��� ����
	hr = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilbuffer);
	if (FAILED(hr))
		return false;


	/*
		���� ���ٽ� ���� (�ɼ�)
	*/
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	//�ȼ� ����� ���ٽ� ���� ó��
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	//�ȼ� �ĸ�� ���ٽ� ���� ó��
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	
	//���� ���ٽ� ���� ����
	hr = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(hr))
		return false;

	//����̽� ���ؽ�Ʈ�� ���� ���ٽ� ���� ����.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	
	
	/*
		���� ���ٽ� �� �����
	*/
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// ����-���ٽ� �並 ����.
	hr = m_device->CreateDepthStencilView(m_depthStencilbuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(hr))
		return false;


	/*
		����̽� ���ؽ�Ʈ�� ����Ÿ�ٺ� �� ���̽��ٽǺ並 ����.
	*/
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	
	/*
		�����Ͷ���¡ ���� (�ɼ�)
		- ������ ��� �׸��� ���� ( ���̾�������, 2 side ������ ���.. )
	*/

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	//����̽��� �����Ͷ���¡ ���� ����.
	hr = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(hr))
		return false;
	
	//����̽� ���ؽ�Ʈ�� �����Ͷ���¡ ���� ����.
	m_deviceContext->RSSetState(m_rasterState);

	

	/*
		����Ʈ ���� 
	*/    
	viewport.Width = (FLOAT)screenWidth;
	viewport.Height = (FLOAT)screenHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    m_deviceContext->RSSetViewports( 1, &viewport );


	/*
		���� ��� ( projection matrix ) ����
		- 3d ȭ���� 2d ����Ʈ �������� ��ȯ
		- ���纻���� ���̴����� ��� �� �� �ְ� �Ѵ�.
	*/
	float fieldOfView = (float)D3DX_PI / 4.0f;
	float screenAspect = (float)screenWidth / (float)screenHeight;

	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);


	/*
		���� ��� ( world matrix ) ����
		- ������Ʈ���� 3d ��ǥ�� ��ȯ�ϱ� ���� ��� 
		- �̵� / ȸ�� / ũ�� �� ���.
		- ���纻���� ���̴��� ��� �� �� �ְ� �Ѵ�.
	*/
	// ���� ����� ���� ��ķ� �ʱ�ȭ
	D3DXMatrixIdentity(&m_worldMatrix);


	/*
		�� ��� ( view matrix )
		- ī�޶󿡼� ���� �����ϱ� ���� �ϴ� ����.
	*/


	/*
		���翵 ��� ( Orthographic matix ���� )
		- ���ٰ��� ���� ���
		- 2d �̹����� UI ���� ǥ���� �� ���.
	*/
	// 2D �������� ���� ���翵 ����� ����.
	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);
	
	return true;
}

void graphicD3d::Release()
{
	//�������� �ݵ�� â ���� ������ ���� �Ѵ�.
	if (m_swapChain)			m_swapChain->SetFullscreenState(false, NULL);
	//�������� ����̽� ���ؽ�Ʈ ���¸� ���� �Ѵ�.
	if (m_deviceContext)		m_deviceContext->ClearState();

	if (m_rasterState)			m_rasterState->Release();
	if (m_depthStencilView)		m_depthStencilView->Release();
	if (m_depthStencilState)	m_depthStencilState->Release();
	if (m_depthStencilbuffer)	m_depthStencilbuffer->Release();
										
	if (m_renderTargetView)		m_renderTargetView->Release();
	if (m_swapChain)			m_swapChain->Release();
	if (m_deviceContext)		m_deviceContext->Release();
	if (m_device)				m_device->Release();
}

void graphicD3d::BegineScene()
{
	//red,green,blue,alpha
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; 

	//����� ���� ����
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, ClearColor);
	//���̹��� ����
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void graphicD3d::EndScene()
{
	//����� ������ ȭ�鿡 ���
	if (m_vsync)
	{
		//���ΰ�ħ ���� ����
		m_swapChain->Present(1, 0);
	}
	else
	{
		//���ΰ�ħ ������ �ý��� ���ɴ�� ������
		m_swapChain->Present(0, 0);
	}
}


SGAP_END