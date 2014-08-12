#include "DXUT.h"
#include "MeshLoader.h"

#include <fstream>

using std::ifstream;

MeshLoader::MeshLoader()
{
}

MeshLoader::~MeshLoader()
{
}

/*
*/
void MeshLoader::LoadModelFromFile(LPCWSTR wfilename)
{
	ifstream	fin;
	char		dat;
	char		datLine[128] = {};
	XMFLOAT3	f3;
	

	fin.open(wfilename);

	// 파일 읽기 실패
	if (fin.fail())
		return;
	
	fin.get(dat);
	while (!fin.eof())
	{
		//
		if (dat == '#')
		{
			fin.getline(datLine, 128);
		}

		//vertex 정보 읽기
		if (dat == 'v')
		{
			fin >> f3.x >> f3.y >> f3.z;
			vertexList.push_back(f3);
		}

		if (dat == 'f')
		{
			fin >> f3.x >> f3.y >> f3.z;
			indexList.push_back(f3);
		}

		fin.get(dat);
	}

	fin.close();
}

/*
*/
void MeshLoader::Initialize()
{
	HRESULT hr = S_OK;

	/*
		VERTEX SHADER
	*/
	ID3DBlob* pVSblob = nullptr;
	// FX 파일 중 VertexShader 로드
	hr = ComplieShaderFromFile(L"basicShader.fx", "VS", "vs_4_0", &pVSblob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Load FX file FAILED", L"VERTEX SHADER ERROR", MB_OK);
		return;
	}

	// Vertex Shader 생성
	hr = DXUTGetD3D11Device()->CreateVertexShader(pVSblob->GetBufferPointer(), pVSblob->GetBufferSize(), NULL, &m_VertexShader);
	if (FAILED(hr))
	{
		pVSblob->Release();
		return;
	}


	/*
		VERTEX LAYOUT
	*/
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Layout 구조체 정보를 바탕으로 VertexLayout 을 생성.
	hr = DXUTGetD3D11Device()->CreateInputLayout(layout, numElements, pVSblob->GetBufferPointer(), pVSblob->GetBufferSize(), &m_VertexLayout);
	pVSblob->Release();
	if (FAILED(hr))
		return;

	// Device Context 로 VertexLayout을 Input Assembler 에 연결
	DXUTGetD3D11DeviceContext()->IASetInputLayout(m_VertexLayout);

	
	/*
		PIXEL SHADER
	*/
	ID3DBlob* pPSblob = nullptr;
	// FX 파일 중 PixelShader 로드
	hr = ComplieShaderFromFile(L"basicShader.fx", "PS", "ps_4_0", &pPSblob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Load FX file FAILED", L"PIXEL SHADER ERROR", MB_OK);
		return;
	}

	// Pixel Shader 생성
	hr = DXUTGetD3D11Device()->CreatePixelShader(pPSblob->GetBufferPointer(), pPSblob->GetBufferSize(), NULL, &m_PixelShader);
	pPSblob->Release();
	if (FAILED(hr))
		return;


	/*
		VERTEX BUFFER
	*/
	D3D11_BUFFER_DESC buffdesc;
	ZeroMemory(&buffdesc, sizeof(buffdesc));
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.ByteWidth = sizeof(VERTEXFORMAT)* vertexList.size();
	buffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffdesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = &vertexList[0];

	hr = DXUTGetD3D11Device()->CreateBuffer(&buffdesc, &initData, &m_VertexBuffer);
	if (FAILED(hr))
		return;

	UINT stride = sizeof(VERTEXFORMAT);
	UINT offset = 0;
	DXUTGetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	DXUTGetD3D11DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


/*
*/
void MeshLoader::Render()
{
	DXUTGetD3D11DeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	DXUTGetD3D11DeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
	DXUTGetD3D11DeviceContext()->Draw(vertexList.size(), 0);
}

/*
*/
void MeshLoader::Release()
{
	if (m_VertexBuffer) m_VertexBuffer->Release();
	if (m_VertexShader) m_VertexShader->Release();
	if (m_PixelShader) m_PixelShader->Release();
	if (m_VertexLayout) m_VertexLayout->Release();
}


/*
*/
HRESULT MeshLoader::ComplieShaderFromFile(WCHAR* wFilename, LPCSTR strEntry, LPCSTR strShaderMdl, ID3DBlob** ppblob)
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