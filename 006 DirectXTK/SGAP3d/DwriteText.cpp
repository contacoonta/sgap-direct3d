
#include "stdafx.h"
#include "dxut.h"
#include "FW1Precompiled.h"
#include "SDKmisc.cpp"
#include "Mesh.h"
#include "DwriteText.h"

#include <string>


//#pragma comment (lib, "d3d10_1.lib")
//#pragma comment (lib, "dxgi.lib")
//#pragma comment (lib, "d2d1.lib")
//#pragma comment (lib, "dwrite.lib")
//
//
//typedef struct Vertex2d
//{
//	XMFLOAT3 pos;
//	XMFLOAT2 texCoord;
//	XMFLOAT3 normal;
//
//}VERTEX2D;
//
//
////Create effects constant buffer's structure//
//struct cbPerObject
//{
//	XMMATRIX  WVP;
//	XMMATRIX World;
//};
//cbPerObject cbPerObj;



DwriteText::DwriteText()
{	
	Initialize();
}

DwriteText::~DwriteText()
{
	Release();
}

HRESULT DwriteText::Initialize()
{
	HRESULT hr = S_OK;


	hr = FW1CreateFactory(FW1_VERSION, &m_factory);
	hr = m_factory->CreateFontWrapper(DXUTGetD3D11Device(), L"Arial", &m_fontwrap);



	//
	//// DXGI Adapter 持失
	//IDXGIFactory1 *DXGIFactory;
	//hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&DXGIFactory);
	//hr = DXGIFactory->EnumAdapters1(0, &Adapter);
	//DXGIFactory->Release();


	//// Direc3D 10.1 Device 持失
	//hr = D3D10CreateDevice1(Adapter, D3D10_DRIVER_TYPE_HARDWARE, NULL,D3D10_CREATE_DEVICE_BGRA_SUPPORT,
	//	D3D10_FEATURE_LEVEL_9_3, D3D10_1_SDK_VERSION, &d3d101Device	);	

	//// Direct3D 10.1 引 11 硲発遂 Shared Texture 持失
	//D3D11_TEXTURE2D_DESC sharedTexDesc;	
	//ZeroMemory(&sharedTexDesc, sizeof(sharedTexDesc));
	//sharedTexDesc.Width = LayoutWidth;
	//sharedTexDesc.Height = LayoutHeight;
	//sharedTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	//sharedTexDesc.MipLevels = 1;	
	//sharedTexDesc.ArraySize = 1;
	//sharedTexDesc.SampleDesc.Count = 1;
	//sharedTexDesc.Usage = D3D11_USAGE_DEFAULT;
	//sharedTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;	
	//sharedTexDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;	

	//hr = DXUTGetD3D11Device()->CreateTexture2D(&sharedTexDesc, NULL, &sharedTex11);	

	//// Get the keyed mutex for the shared texture (for D3D11)///////////////////////////////////////////////////////////////
	//hr = sharedTex11->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&keyedMutex11);	

	//// Get the shared handle needed to open the shared texture in D3D10.1///////////////////////////////////////////////////
	//IDXGIResource *sharedResource10;
	//HANDLE sharedHandle10;	

	//hr = sharedTex11->QueryInterface(__uuidof(IDXGIResource), (void**)&sharedResource10);

	//hr = sharedResource10->GetSharedHandle(&sharedHandle10);	

	//sharedResource10->Release();

	//// Open the surface for the shared texture in D3D10.1///////////////////////////////////////////////////////////////////
	//IDXGISurface1 *sharedSurface10;	

	//hr = d3d101Device->OpenSharedResource(sharedHandle10, __uuidof(IDXGISurface1), (void**)(&sharedSurface10));

	//hr = sharedSurface10->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&keyedMutex10);	

	//// Create D2D factory///////////////////////////////////////////////////////////////////////////////////////////////////
	//ID2D1Factory *D2DFactory;	
	//hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), (void**)&D2DFactory);	

	//D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;

	//ZeroMemory(&renderTargetProperties, sizeof(renderTargetProperties));

	//renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
	//renderTargetProperties.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);	

	//hr = D2DFactory->CreateDxgiSurfaceRenderTarget(sharedSurface10, &renderTargetProperties, &D2DRenderTarget);

	//sharedSurface10->Release();
	//D2DFactory->Release();	

	//// Create a solid color brush to draw something with		
	//hr = D2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), &Brush);

	////DirectWrite///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
	//	reinterpret_cast<IUnknown**>(&DWriteFactory));

	//hr = DWriteFactory->CreateTextFormat(
	//	L"Script",
	//	NULL,
	//	DWRITE_FONT_WEIGHT_REGULAR,
	//	DWRITE_FONT_STYLE_NORMAL,
	//	DWRITE_FONT_STRETCH_NORMAL,
	//	24.0f,
	//	L"en-us",
	//	&TextFormat
	//	);

	//hr = TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	//hr = TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	//d3d101Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);


	///*
	//	Shader Effects
	//*/
	//WCHAR strpathW[256] = {};
	//DXUTFindDXSDKMediaFileCch(strpathW, sizeof(strpathW) / sizeof(WCHAR), L"shaders\\effects.fx");
	//
	//ID3D10Blob* D2D_VS_Buffer = nullptr;
	//hr = DXUTCompileFromFile(strpathW, nullptr, "D2D_VS", "vs_4_0", 0, 0, &D2D_VS_Buffer);
	//hr = DXUTGetD3D11Device()->CreateVertexShader(D2D_VS_Buffer->GetBufferPointer(), D2D_VS_Buffer->GetBufferSize(), NULL, &D2D_VS);
	//SAFE_RELEASE(D2D_VS_Buffer);

	//ID3D10Blob* D2D_PS_Buffer = nullptr;
	//hr = DXUTCompileFromFile(strpathW, nullptr, "D2D_PS", "ps_4_0", 0, 0, &D2D_PS_Buffer);
	//hr = DXUTGetD3D11Device()->CreatePixelShader(D2D_PS_Buffer->GetBufferPointer(), D2D_PS_Buffer->GetBufferSize(), NULL, &D2D_PS);
	//SAFE_RELEASE(D2D_PS_Buffer);


	///*
	//	Blend modes
	//*/
	//D3D11_BLEND_DESC blendDesc;
	//ZeroMemory(&blendDesc, sizeof(blendDesc));

	//D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	//ZeroMemory(&rtbd, sizeof(rtbd));

	//rtbd.BlendEnable = true;
	//rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
	//rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	//rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	//rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	//rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	//rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	//rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	//blendDesc.AlphaToCoverageEnable = false;
	//blendDesc.RenderTarget[0] = rtbd;

	//hr = DXUTGetD3D11Device()->CreateBlendState(&blendDesc, &d2dTransparency);

	//ZeroMemory(&rtbd, sizeof(rtbd));

	//rtbd.BlendEnable = true;
	//rtbd.SrcBlend = D3D11_BLEND_INV_SRC_ALPHA;
	//rtbd.DestBlend = D3D11_BLEND_SRC_ALPHA;
	//rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	//rtbd.SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	//rtbd.DestBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	//rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	//rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	//blendDesc.AlphaToCoverageEnable = false;
	//blendDesc.RenderTarget[0] = rtbd;

	//hr = DXUTGetD3D11Device()->CreateBlendState(&blendDesc, &Transparency);
	//
	//
	////Create the buffer to send to the cbuffer in effect file
	//D3D11_BUFFER_DESC cbbd;
	//ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	//cbbd.Usage = D3D11_USAGE_DEFAULT;
	//cbbd.ByteWidth = sizeof(cbPerObject);
	//cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//cbbd.CPUAccessFlags = 0;
	//cbbd.MiscFlags = 0;

	//hr = DXUTGetD3D11Device()->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);

	//
	//// Describe the Sample State
	//D3D11_SAMPLER_DESC sampDesc;
	//ZeroMemory(&sampDesc, sizeof(sampDesc));
	//sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	//sampDesc.MinLOD = 0;
	//sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	////Create the Sample State
	//hr = DXUTGetD3D11Device()->CreateSamplerState(&sampDesc, &TexSamplerState);


	///*
	//	Rasterizer 
	//*/
	//D3D11_RASTERIZER_DESC cmdesc;

	//ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	//cmdesc.FillMode = D3D11_FILL_SOLID;
	//cmdesc.CullMode = D3D11_CULL_BACK;
	//cmdesc.FrontCounterClockwise = true;
	//hr = DXUTGetD3D11Device()->CreateRasterizerState(&cmdesc, &CCWcullMode);
	//cmdesc.FrontCounterClockwise = false;
	//hr = DXUTGetD3D11Device()->CreateRasterizerState(&cmdesc, &CWcullMode);
	//

	///*
	//*/
	//hr = Init2dScreenTexture();

	return hr;
}

/*
*/
//HRESULT DwriteText::Init2dScreenTexture()
//{
//	HRESULT hr = S_OK;
//
//	//Create the vertex buffer
//	VERTEX2D v[] =
//	{
//		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f) },
//		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 1.0f, -1.0f)},
//		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, -1.0f)},
//		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(1.0f, -1.0f, -1.0f) },
//	};
//
//	DWORD indices[] = 
//	{
//		// Front Face
//		0, 1, 2,
//		0, 2, 3,
//	};
//
//	D3D11_BUFFER_DESC indexBufferDesc;
//	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
//
//	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	indexBufferDesc.ByteWidth = sizeof(DWORD)* 2 * 3;
//	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	indexBufferDesc.CPUAccessFlags = 0;
//	indexBufferDesc.MiscFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA iinitData;
//
//	iinitData.pSysMem = indices;
//	hr = DXUTGetD3D11Device()->CreateBuffer(&indexBufferDesc, &iinitData, &d2dIndexBuffer);
//
//
//	D3D11_BUFFER_DESC vertexBufferDesc;
//	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
//
//	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	vertexBufferDesc.ByteWidth = sizeof(VERTEX2D)* 4;
//	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vertexBufferDesc.CPUAccessFlags = 0;
//	vertexBufferDesc.MiscFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA vertexBufferData;
//
//	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
//	vertexBufferData.pSysMem = v;
//	hr = DXUTGetD3D11Device()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &d2dVertBuffer);
//
//	//Create A shader resource view from the texture D2D will render to,
//	//So we can use it to texture a square which overlays our scene
//	hr = DXUTGetD3D11Device()->CreateShaderResourceView(sharedTex11, NULL, &d2dTexture);
//
//	return hr;
//}

/*
*/
void DwriteText::Render(std::wstring wtext, int numex)
{
	HRESULT hr = S_OK;

	//Create our string
	std::wstring str;
	str = wtext + std::to_wstring(numex);
	m_fontwrap->DrawString(	DXUTGetD3D11DeviceContext(),
							str.c_str(),// String
							24.0f,// Font size
							10,// X position
							10,// Y position
							0xff0099ff,// Text color, 0xAaBbGgRr
							FW1_NOGEOMETRYSHADER // Flags	
							);
	
	//DXUTGetD3D11DeviceContext()->VSSetShader(D2D_VS, 0, 0);
	//DXUTGetD3D11DeviceContext()->PSSetShader(D2D_PS, 0, 0);

	////Release the D3D 11 Device
	//keyedMutex11->ReleaseSync(0);
	////Use D3D10.1 device
	//keyedMutex10->AcquireSync(0, 5);	

	////Draw D2D content		
	//D2DRenderTarget->BeginDraw();	
	////Clear D2D Background
	//D2DRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));
	//{
	//	//Create our string
	//	std::wstring str;
	//	str = wtext + std::to_wstring(numex);

	//	//Set the Font Color
	//	D2D1_COLOR_F FontColor = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);
	//	//Set the brush color D2D will use to draw with
	//	Brush->SetColor(FontColor);
	//	//Create the D2D Render Area
	//	D2D1_RECT_F layoutRect = D2D1::RectF(0, 0, LayoutWidth, LayoutHeight);

	//	//Draw the Text
	//	D2DRenderTarget->DrawText(str.c_str(), wcslen(str.c_str()), TextFormat, layoutRect, Brush);
	//}
	//D2DRenderTarget->EndDraw();	

	////Release the D3D10.1 Device
	//keyedMutex10->ReleaseSync(1);
	////Use the D3D11 Device
	//keyedMutex11->AcquireSync(1, 5);

	////Use the shader resource representing the direct2d render target
	////to texture a square which is rendered in screen space so it
	////overlays on top of our entire scene. We use alpha blending so
	////that the entire background of the D2D render target is "invisible",
	////And only the stuff we draw with D2D will be visible (the text)

	////Set the blend state for D2D render target texture objects
	//DXUTGetD3D11DeviceContext()->OMSetBlendState(d2dTransparency, NULL, 0xffffffff);

	////Set the d2d Index buffer
	//DXUTGetD3D11DeviceContext()->IASetIndexBuffer(d2dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	////Set the d2d vertex buffer
	//UINT stride = sizeof( VERTEX2D );
	//UINT offset = 0;
	//DXUTGetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &d2dVertBuffer, &stride, &offset);

	//XMMATRIX WVP =  XMMatrixIdentity();
	//cbPerObj.WVP = XMMatrixTranspose(WVP);	
	//cbPerObj.World = XMMatrixIdentity();

	//DXUTGetD3D11DeviceContext()->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	//
	//DXUTGetD3D11DeviceContext()->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	//DXUTGetD3D11DeviceContext()->PSSetShaderResources(0, 1, &d2dTexture);
	//DXUTGetD3D11DeviceContext()->PSSetSamplers(0, 1, &TexSamplerState);

	////DXUTGetD3D11DeviceContext()->RSSetState(CWcullMode);
	//DXUTGetD3D11DeviceContext()->DrawIndexed(6, 0, 0);
}

void DwriteText::Release()
{
	SAFE_RELEASE(m_factory);
	SAFE_RELEASE(m_fontwrap);

	/*SAFE_RELEASE(keyedMutex11);
	SAFE_RELEASE(keyedMutex10);
	SAFE_RELEASE(D2DRenderTarget);
	SAFE_RELEASE(Brush);
	SAFE_RELEASE(sharedTex11);
	SAFE_RELEASE(DWriteFactory);
	SAFE_RELEASE(TextFormat);
	SAFE_RELEASE(d2dVertBuffer);
	SAFE_RELEASE(d2dIndexBuffer);
	SAFE_RELEASE(cbPerObjectBuffer);
	SAFE_RELEASE(d2dTexture);
	SAFE_RELEASE(TexSamplerState);
	SAFE_RELEASE(CCWcullMode);
	SAFE_RELEASE(CWcullMode);	
	SAFE_RELEASE(D2D_VS);
	SAFE_RELEASE(D2D_PS);
	SAFE_RELEASE(d2dTransparency);
	SAFE_RELEASE(Transparency);
	SAFE_RELEASE(d3d101Device);
	SAFE_RELEASE(Adapter);*/
}