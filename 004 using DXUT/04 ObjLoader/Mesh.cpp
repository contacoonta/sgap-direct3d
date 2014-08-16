#include "DXUT.h"
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
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// Shader Layout 설정
	hr = CompileShader::Create(&m_compileshader, L"lightShader.fx", layoutPN, ARRAYSIZE(layoutPN));
	if (FAILED(hr))
		return hr;
}

HRESULT Mesh::BuildCube()
{
	HRESULT hr;		
	
	hr = Initialize();

	hr = ObjLoader::CreateModelFromFile(&m_objloader, nullptr, *this);

	return hr;
}


/*

*/
HRESULT Mesh::BuildFromObj(LPCWSTR wfilename)
{
	HRESULT hr;

	hr = Initialize();

	ObjLoader::CreateModelFromFile(&m_objloader, wfilename, *this);

	return hr;
}

void Mesh::Update()
{
	/*
		World, View, Projection 매트릭스 설정
	*/
	
	//WORLD MATRIX
	m_world			= XMMatrixIdentity();

	//VIEW MATRIX
	XMVECTOR Eye	= XMVectorSet(15.0f, 30.0f, -30.0f, 0.0f);
	XMVECTOR LookAt = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up		= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_view			= XMMatrixLookAtLH(Eye, LookAt, Up);

	//PROJECTION MATRIX
	LONG width		= DXUTGetWindowWidth();
	LONG height		= DXUTGetWindowHeight();
	m_projection	= XMMatrixPerspectiveFovLH(XM_PIDIV2, (FLOAT)width / (FLOAT)height, 0.01f, 100.0f);
}

void Mesh::Render()
{
	XMFLOAT4 LitDir = XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f);
	XMFLOAT4 LitCol = XMFLOAT4(0.9f, 0.2f, 0.3f, 1.0f);
	
	/*
		Constant Buffer 연결
	*/
	ConstantBuffer cb;
	cb.world = XMMatrixTranspose(m_world);
	cb.view = XMMatrixTranspose(m_view);
	cb.projection = XMMatrixTranspose(m_projection);
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