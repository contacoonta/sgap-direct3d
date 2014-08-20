#include "DXUT.h"
#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}


HRESULT Mesh::Initialize()
{

	HRESULT hr = S_OK;

	return hr;
}


void Mesh::Update()
{
	//WORLD MATRIX
	m_world = XMMatrixIdentity();
}

void Mesh::Render()
{
	DXUTGetD3D11DeviceContext()->DrawIndexed(m_indexCnt, 0, 0);
}

void Mesh::Release()
{
	if (m_vertexbuffer) m_vertexbuffer->Release();
	if (m_indexbuffer) m_indexbuffer->Release();
}