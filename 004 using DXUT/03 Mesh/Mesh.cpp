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
		
	
	D3D11_INPUT_ELEMENT_DESC layoutP[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// Shader Layout 설정
	hr = CompileShader::Create(&m_compileshader,L"basicShader.fx", layoutP, ARRAYSIZE(layoutP));
	if (FAILED(hr))
		return hr;
	

	/*
		vertex list 로 vertex buffer 만들기
	*/
	SIMPLEVERTEX vertices[] =
	{
		XMFLOAT3(0.0f, 0.5f, 0.5f),
		XMFLOAT3(0.5f, -0.5f, 0.5f),
		XMFLOAT3(-0.5f, -0.5f, 0.5f),
		
		XMFLOAT3(-1.0f, 0.5f, 0.5f),
		XMFLOAT3(0.0f, 0.5f, 0.5f),
		XMFLOAT3(-0.5f, -0.5f, 0.5f),

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

	DXUTGetD3D11DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void Mesh::Render()
{
	m_compileshader->Render();
	DXUTGetD3D11DeviceContext()->Draw(6, 0);
}

void Mesh::Release()
{
	if (m_compileshader) CompileShader::Delete(&m_compileshader);
	if ( m_vertexbuffer) m_vertexbuffer->Release();	
}