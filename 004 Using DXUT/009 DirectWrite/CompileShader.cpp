#include "DXUT.h"
#include "SDKmisc.h"


#include "CompileShader.h"



CompileShader::CompileShader()
{
}


CompileShader::~CompileShader()
{
}

HRESULT CompileShader::Create(CompileShader** ppshader, WCHAR* wfilename, WCHAR* wtexturefilename)
{
	HRESULT hr = S_OK;

	if (*ppshader)
		return S_FALSE;

	*ppshader = new CompileShader;

	WCHAR strpathW[256] = {};
	DXUTFindDXSDKMediaFileCch(strpathW, sizeof(strpathW) / sizeof(WCHAR), wfilename);
	hr = (*ppshader)->Initialize(strpathW);


	return hr;
	
}

void CompileShader::Delete(CompileShader** ppshader)
{
	if (*ppshader == nullptr)
		return;

	(*ppshader)->Release();
	
	delete *ppshader;
	*ppshader = nullptr;
}


/*
*/
void CompileShader::RenderPrepare( const void* psrcData )
{
	DXUTGetD3D11DeviceContext()->UpdateSubresource(m_constantbuffer, 0, NULL, psrcData, 0, 0);

	DXUTGetD3D11DeviceContext()->VSSetShader(m_vertexshader, NULL, 0);
	DXUTGetD3D11DeviceContext()->VSSetConstantBuffers(0, 1, &m_constantbuffer);

	DXUTGetD3D11DeviceContext()->PSSetShader(m_pixelshader, NULL, 0);
	DXUTGetD3D11DeviceContext()->PSSetConstantBuffers(0, 1, &m_constantbuffer);
	
	DXUTGetD3D11DeviceContext()->PSSetSamplers(0, 1, &m_samplerLinear);
}


/*
*/
HRESULT CompileShader::Initialize(WCHAR* wfilename)
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC layoutPN[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layoutPN);

	/*
		VERTEX SHADER
	*/
	ID3DBlob* pVSblob = nullptr;
	// FX 파일 중 VertexShader 로드
	hr = ComplieShaderFromFile(wfilename, "VS", "vs_4_0", &pVSblob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Load FX file FAILED", L"ERROR", MB_OK);
		return hr;
	}

	// Vertex CompileShader 생성
	hr = DXUTGetD3D11Device()->CreateVertexShader(pVSblob->GetBufferPointer(), pVSblob->GetBufferSize(), NULL, &m_vertexshader);
	if (FAILED(hr))
	{
		pVSblob->Release();
		return hr;
	}


	/*
		VERTEX LAYOUT
	*/
	// Layout 구조체 정보를 바탕으로 VertexLayout 을 생성.
	hr = DXUTGetD3D11Device()->CreateInputLayout(layoutPN, numElements, pVSblob->GetBufferPointer(), pVSblob->GetBufferSize(), &m_vertexlayout);
	pVSblob->Release();
	if (FAILED(hr))
		return hr;

	// VertexLayout 을 device Context 에 설정.
	DXUTGetD3D11DeviceContext()->IASetInputLayout(m_vertexlayout);


	/*
		PIXEL SHADER
	*/
	ID3DBlob* pPSblob = nullptr;
	// FX 파일 중 PixelShader 로드
	hr = ComplieShaderFromFile(wfilename, "PS", "ps_4_0", &pPSblob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Load FX file FAILED", L"ERROR", MB_OK);
		return hr;
	}

	// Pixel CompileShader 생성
	hr = DXUTGetD3D11Device()->CreatePixelShader(pPSblob->GetBufferPointer(), pPSblob->GetBufferSize(), NULL, &m_pixelshader);
	pPSblob->Release();
	if (FAILED(hr))
	{
		return hr;
	}


	/*
		상수 버퍼 만들기 ( Constant buffer )
	*/
	D3D11_BUFFER_DESC buffdesc;
	ZeroMemory(&buffdesc, sizeof(buffdesc));
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.ByteWidth = sizeof(CONSTANTBUFFER);
	buffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.CPUAccessFlags = 0;

	// 상수 버퍼 생성
	hr = DXUTGetD3D11Device()->CreateBuffer(&buffdesc, NULL, &m_constantbuffer);
	if (FAILED(hr))
		return hr;



	//텍스쳐 샘플러 설정
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = DXUTGetD3D11Device()->CreateSamplerState(&sampDesc, &m_samplerLinear);
	if (FAILED(hr))
		return hr;

	return hr;
}


/*
*/
void CompileShader::Release()
{
	SAFE_RELEASE(m_samplerLinear);
	SAFE_RELEASE(m_constantbuffer);
	SAFE_RELEASE(m_vertexshader);
	SAFE_RELEASE(m_vertexlayout);
	SAFE_RELEASE(m_pixelshader);
}


/*
*/
HRESULT CompileShader::ComplieShaderFromFile(WCHAR* wFilename, LPCSTR strEntry, LPCSTR strShaderMdl, ID3DBlob** ppblob)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	hr = DXUTCompileFromFile(wFilename, nullptr, strEntry, strShaderMdl, dwShaderFlags, 0, ppblob);
	
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}