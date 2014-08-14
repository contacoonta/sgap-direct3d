#include "DXUT.h"
#include "Mesh.h"

#include "CompileShader.h"






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
	hr = CompileShader::Create(&m_compileshader, L"lightShader.fx", layoutPN, ARRAYSIZE(layoutPN));
	if (FAILED(hr))
		return hr;
	

	/*
		vertex list 로 vertex buffer 만들기
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

	D3D11_BUFFER_DESC buffdesc;
	ZeroMemory(&buffdesc, sizeof(buffdesc));
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.ByteWidth = sizeof(SIMPLEVERTEX) * numVertices;
	buffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffdesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertices;

	hr = DXUTGetD3D11Device()->CreateBuffer(&buffdesc, &initData, &m_vertexbuffer);
	if (FAILED(hr))
		return hr;

	UINT stride = sizeof(SIMPLEVERTEX);
	UINT offset = 0;
	DXUTGetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_vertexbuffer, &stride, &offset);


	/*
		인덱스 버퍼 만들기
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
	buffdesc.ByteWidth = sizeof(WORD)* numIndices;
	buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffdesc.CPUAccessFlags = 0;
	initData.pSysMem = indices;
	// 인덱스 버퍼 생성
	hr = DXUTGetD3D11Device()->CreateBuffer(&buffdesc, &initData, &m_indexbuffer);
	if (FAILED(hr))
		return hr;
	// 인풋 어셈블러에 인덱스 버퍼 설정
	DXUTGetD3D11DeviceContext()->IASetIndexBuffer(m_indexbuffer, DXGI_FORMAT_R16_UINT, 0);
	
	DXUTGetD3D11DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	/*
		상수 버퍼 만들기 ( Constant buffer )
	*/
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.ByteWidth = sizeof(ConstantBuffer);
	buffdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffdesc.CPUAccessFlags = 0;

	// 상수 버퍼 생성
	hr = DXUTGetD3D11Device()->CreateBuffer(&buffdesc, NULL, &m_constantbuffer);
	if (FAILED(hr))
		return hr;



	/*
		World, View, Projection 매트릭스 초기화
	*/
	
	//WORLD MATRIX
	m_world			= XMMatrixIdentity();


	//VIEW MATRIX
	XMVECTOR Eye	= XMVectorSet(3.0f, 5.0f, -5.0f, 0.0f);
	XMVECTOR LookAt = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up		= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_view			= XMMatrixLookAtLH(Eye, LookAt, Up);

	//PROJECTION MATRIX
	m_projection	= XMMatrixPerspectiveFovLH(XM_PIDIV2, (FLOAT)1024 / (FLOAT)768, 0.01f, 100.0f);

}

void Mesh::Update()
{
	XMFLOAT4 LitDir[] =
	{
		XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f)
	};


}

void Mesh::Render()
{
	m_compileshader->RenderPrepare();
	DXUTGetD3D11DeviceContext()->Draw(6, 0);
}

void Mesh::Release()
{
	if (m_compileshader) CompileShader::Delete(&m_compileshader);
	
	if (m_vertexbuffer) m_vertexbuffer->Release();	
	if (m_indexbuffer) m_indexbuffer->Release();
	if (m_constantbuffer) m_constantbuffer->Release();
	
}