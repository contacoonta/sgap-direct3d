#include "DXUT.h"
#include "Mesh.h"


Mesh::Mesh()
{
	XMStoreFloat4x4(&m_world, XMMatrixIdentity());

	Initialize();
}

Mesh::~Mesh()
{
	Release();
}


Mesh* Mesh::Clone()
{
	Mesh* mesh = new Mesh;

	mesh->m_vertexBuff = m_vertexBuff;
	mesh->m_indexBuff = m_indexBuff;
	mesh->m_indexCnt = m_indexCnt;
	XMStoreFloat4x4(&mesh->m_world, XMMatrixIdentity());
	mesh->m_bCloned = TRUE;

	return mesh;
}



HRESULT Mesh::Initialize()
{
	HRESULT hr = S_OK;

	return hr;
}

void Mesh::Render( /*CompileShader* pshader*/)
{
	DXUTGetD3D11DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//IA ���ٰ� �ε��� ���� ����
	DXUTGetD3D11DeviceContext()->IASetIndexBuffer(m_indexBuff, DXGI_FORMAT_R32_UINT, 0);

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	//���ؽ� ���۸� IA �� ���� ( 0�� ���� )
	DXUTGetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuff, &stride, &offset);

	// �ε��� ���۷� �׸���
	DXUTGetD3D11DeviceContext()->DrawIndexed(m_indexCnt, 0, 0);
}


void Mesh::Release()
{
	if (m_bCloned)
		return;

	SAFE_RELEASE(m_indexBuff);
	SAFE_RELEASE(m_vertexBuff);
}