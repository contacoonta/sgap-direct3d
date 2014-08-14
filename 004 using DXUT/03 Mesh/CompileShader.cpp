#include "DXUT.h"
#include "CompileShader.h"


CompileShader::CompileShader()
{
}


CompileShader::~CompileShader()
{
}

HRESULT CompileShader::Create(CompileShader** ppshader, WCHAR* wfilename, D3D11_INPUT_ELEMENT_DESC pLayout[], UINT numElements)
{
	if (*ppshader)
		return S_FALSE;

	*ppshader = new CompileShader;

	return (*ppshader)->Initialize(wfilename, pLayout, numElements);
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
}


/*
*/
HRESULT CompileShader::Initialize(WCHAR* wfilename, D3D11_INPUT_ELEMENT_DESC pLayout[], UINT numElements)
{
	HRESULT hr;

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
	hr = DXUTGetD3D11Device()->CreateInputLayout(pLayout, numElements, pVSblob->GetBufferPointer(), pVSblob->GetBufferSize(), &m_vertexlayout);
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

}


/*
*/
void CompileShader::Release()
{
	if (m_constantbuffer) m_constantbuffer->Release();
	if (m_vertexshader) m_vertexshader->Release();
	if (m_vertexlayout) m_vertexlayout->Release();
	if (m_pixelshader) m_pixelshader->Release();
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

	if (pErrorBlob)
		pErrorBlob->Release();

	return S_OK;
}