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


	// 수직동기화 설정 ( true / false )
	m_vsync = vsync;

	//DirectX Graphic Infrastructure 인터페이스 팩토리 만들기.
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(hr))
		return false;
	
	//첫번째 그래픽 카드 어댑터 만들기.
	hr = factory->EnumAdapters(0, &adapter);
	if (FAILED(hr))
		return false;

	//첫번째 모니터의 어댑터 만들기.
	hr = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(hr))
		return false;

	//DXGI_FORMAT_R8G8B8A8_UNORM 모니터 출력 포멧에 맞는 모드의 개수 구하기.
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &nummodes, NULL);
	if (FAILED(hr))
		return false;

	//사용 가능한 모든 모니터와 그래픽카드 조합을 저장할 리스트 생성.
	displayModelist = new DXGI_MODE_DESC[nummodes];
	if (!displayModelist)
		return false;

	//displayModelist 에 값들 채워넣기.
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &nummodes, displayModelist);
	if (FAILED(hr))
		return false;

	//그래픽 카드와 모니터의 화면 너비/높이 조합을 찾기 완료.
	//모니터의 refresh rate 의 분모 와 분자 값을 저장.
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


	//그래픽 카드 정보 가져오기.
	hr = adapter->GetDesc(&adapterDesc);
	if (FAILED(hr))
		return false;

	//그래픽 카드 메모리 크기 가져오기. ( MegaBytes )
	m_videoMemory = UINT(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	
	//그래픽 카드 이름 가져오기.
	lstrcpynW(m_videoDescription, adapterDesc.Description, 128);


	/*
	*/
	// 원하는 정보 조회 끝나면, 해당 리소스들 해제
	delete [] displayModelist;
	displayModelist = nullptr;

	//모니터 출력 어댑터 해제
	adapterOutput->Release();
	adapterOutput = nullptr;

	//그래픽카드 어댑터 해제
	adapter->Release();
	adapter = nullptr;

	//팩토리 해제
	factory->Release();
	factory = nullptr;
	

	/*
		스왑체인 설정
	*/

    ZeroMemory( &swapchainDesc, sizeof( swapchainDesc ) );
	//백버퍼 개수를 1개로 설정.
    swapchainDesc.BufferCount = 1;
	swapchainDesc.BufferDesc.Width = screenWidth;
	swapchainDesc.BufferDesc.Height = screenHeight;
    swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	if (m_vsync) {
		//vsync 수치에 맞춰서 page flip 이 일어난다.
		swapchainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapchainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else {
		//vsync 와 상관없이 최대한 많이 그리게 한다.
		swapchainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	// 백버퍼를 렌더 타겟 용도로 사용 설정.
    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchainDesc.OutputWindow = hwnd;
	//멀티샘플링 설정을 안함.
    swapchainDesc.SampleDesc.Count = 1;
    swapchainDesc.SampleDesc.Quality = 0;
	//전체화면 모드 창 모드 설정.
	if (fullscreen) {
		swapchainDesc.Windowed = FALSE;
	}
	else {
		swapchainDesc.Windowed = TRUE;
	}
	//스캔라인 정렬 및 사용여부를 안함 (unspecified) 으로 설정.
	swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	//출력 후 백버퍼 내용 버림으로 설정.
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//추가 옵션 플랙 사용 안함.
	swapchainDesc.Flags = 0;


	/*
		디바이스 설정
	*/
    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//그래픽 카드가 dx11 지원되면  HARDWARE,
	//실패하면 REFERENCE 로 처리된다. ( 속도는 HARDWARE 대비 1/1000 정도 느림 )
    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

	//directx 버젼 설정
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
		렌더타겟뷰 (render target view) 만들기
	*/
    //백버퍼 포인터를 가져오기.
    ID3D11Texture2D* pBackBuffer = NULL;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if( FAILED( hr ) )
        return false;

	//백버퍼 포인터로 렌더타겟뷰 생성.
    hr = m_device->CreateRenderTargetView( pBackBuffer, NULL, &m_renderTargetView );
    if( FAILED( hr ) )
        return false;

	//렌더타겟 생성되면 백버퍼 포인터 필요없으므로 제거.
    pBackBuffer->Release();
	pBackBuffer = nullptr;



	/*
		깊이버퍼 (depth buffer) 만들기 (옵션)
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

	//깊이버퍼 2D텍스쳐 생성
	hr = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilbuffer);
	if (FAILED(hr))
		return false;


	/*
		깊이 스텐실 정의 (옵션)
	*/
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	//픽셀 전면부 스텐실 연산 처리
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	//픽셀 후면부 스텐실 연산 처리
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	
	//깊이 스텐실 상태 생성
	hr = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(hr))
		return false;

	//디바이스 컨텍스트에 깊이 스텐실 상태 적용.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	
	
	/*
		깊이 스텐실 뷰 만들기
	*/
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// 깊이-스텐실 뷰를 생성.
	hr = m_device->CreateDepthStencilView(m_depthStencilbuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(hr))
		return false;


	/*
		디바이스 컨텍스트에 렌더타겟뷰 와 깊이스텐실뷰를 적용.
	*/
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	
	/*
		래스터라이징 설정 (옵션)
		- 도형을 어떻게 그릴지 결정 ( 와이어프레임, 2 side 렌더링 등등.. )
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

	//디바이스로 래스터라이징 상태 생성.
	hr = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(hr))
		return false;
	
	//디바이스 컨텍스트로 래스터라이징 상태 적용.
	m_deviceContext->RSSetState(m_rasterState);

	

	/*
		뷰포트 생성 
	*/    
	viewport.Width = (FLOAT)screenWidth;
	viewport.Height = (FLOAT)screenHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    m_deviceContext->RSSetViewports( 1, &viewport );


	/*
		투영 행렬 ( projection matrix ) 생성
		- 3d 화면을 2d 뷰포트 공간으로 변환
		- 복사본으로 셰이더에서 사용 할 수 있게 한다.
	*/
	float fieldOfView = (float)D3DX_PI / 4.0f;
	float screenAspect = (float)screenWidth / (float)screenHeight;

	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);


	/*
		월드 행렬 ( world matrix ) 생성
		- 오브젝트들을 3d 좌표로 변환하기 위한 행렬 
		- 이동 / 회전 / 크기 에 사용.
		- 복사본으로 셰이더에 사용 할 수 있게 한다.
	*/
	// 월드 행렬을 단위 행렬로 초기화
	D3DXMatrixIdentity(&m_worldMatrix);


	/*
		뷰 행렬 ( view matrix )
		- 카메라에서 따로 생성하기 위해 일단 생략.
	*/


	/*
		정사영 행렬 ( Orthographic matix 생성 )
		- 원근감이 없는 행렬
		- 2d 이미지나 UI 등을 표현할 때 사용.
	*/
	// 2D 렌더링에 사용될 정사영 행렬을 생성.
	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);
	
	return true;
}

void graphicD3d::Release()
{
	//종료전에 반드시 창 모드로 설정해 놔야 한다.
	if (m_swapChain)			m_swapChain->SetFullscreenState(false, NULL);
	//종료전에 디바이스 컨텍스트 상태를 정리 한다.
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

	//백버퍼 내용 정리
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, ClearColor);
	//깊이버퍼 정리
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void graphicD3d::EndScene()
{
	//백버퍼 내용을 화면에 출력
	if (m_vsync)
	{
		//새로고침 비율 고정
		m_swapChain->Present(1, 0);
	}
	else
	{
		//새로고침 비율을 시스템 성능대로 빠르게
		m_swapChain->Present(0, 0);
	}
}


SGAP_END