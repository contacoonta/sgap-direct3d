#include "DXUT.h"
#include "SDKmisc.h"

#include "Mesh.h"
#include "CompileShader.h"
#include "ObjLoader.h"






Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}


HRESULT Mesh::Initialize()
{
	HRESULT hr;

	D3D11_INPUT_ELEMENT_DESC layoutPN[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// Shader Layout 설정
	hr = CompileShader::Create(&m_compileshader, L"shaders\\lightShader.fx", layoutPN, ARRAYSIZE(layoutPN));
	if (FAILED(hr))
		return hr;
}

HRESULT Mesh::BuildCube()
{
	HRESULT hr = S_OK;		
	
	hr = Initialize();
	if (FAILED(hr))
		return hr;

	hr = ObjLoader::CreateModelFromFile(&m_objloader, nullptr, *this);
	if (FAILED(hr))
		return hr;

	return hr;
}


/*

*/
HRESULT Mesh::BuildFromObj(LPCWSTR wfilename)
{
	HRESULT hr;

	hr = Initialize();

	WCHAR strpathW[256] = {};
	DXUTFindDXSDKMediaFileCch(strpathW, sizeof(strpathW) / sizeof(WCHAR), wfilename);

	ObjLoader::CreateModelFromFile(&m_objloader, strpathW, *this);

	return hr;
}

void Mesh::Update()
{
	//WORLD MATRIX
	m_world			= XMMatrixIdentity();
}

void Mesh::Render(XMMATRIX& view, XMMATRIX& proj)
{
	XMFLOAT4 LitDir = XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f);
	XMFLOAT4 LitCol = XMFLOAT4(0.9f, 0.2f, 0.3f, 1.0f);
	
	/*
		Constant Buffer 연결
	*/
	ConstantBuffer cb;
	XMStoreFloat4x4(&(cb.world), XMMatrixTranspose(m_world));
	XMStoreFloat4x4(&(cb.view), XMMatrixTranspose(view));
	XMStoreFloat4x4(&(cb.projection), XMMatrixTranspose(proj));
	
	cb.litDir = LitDir;
	cb.litCol = LitCol;
	
	m_compileshader->RenderPrepare(&cb);
	
	DXUTGetD3D11DeviceContext()->DrawIndexed(m_indexCnt, 0, 0);
}

void Mesh::Release()
{
	if (m_objloader) ObjLoader::Delete(&m_objloader);
	if (m_compileshader) CompileShader::Delete(&m_compileshader);
	
	if (m_vertexbuffer) m_vertexbuffer->Release();	
	if (m_indexbuffer) m_indexbuffer->Release();
	
}