#include "DXUT.h"
#include "Mesh.h"
#include "MeshStatic.h"
#include "CompileShader.h"

MeshStatic::MeshStatic()
{
	Initialize();
}

MeshStatic::~MeshStatic()
{
	Release();
}


/*
*/
Mesh* MeshStatic::Clone()
{
	Mesh* mesh = new MeshStatic;

	// 강제 형변환
	/*MeshStatic* ms = static_cast<MeshStatic*>(mesh);
	MeshStatic* ms = (MeshStatic*)mesh;*/

	// 동적 형변환 - 실패시 NULL 반환
	MeshStatic* ms = dynamic_cast<MeshStatic*>(mesh);
	if (ms == nullptr)
	{
		delete mesh;
		return nullptr;
	}

	ms->m_vertexBuff = m_vertexBuff;
	ms->m_indexBuff = m_indexBuff;
	ms->m_indexCnt = m_indexCnt;
	XMStoreFloat4x4(&ms->m_world, XMMatrixIdentity());
	ms->m_bCloned = TRUE;

	return mesh;
}


/*
*/
HRESULT MeshStatic::Initialize()
{
	return S_OK;
}

void MeshStatic::Render( CompileShader* pshader)
{
	DXUTGetD3D11DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//IA 에다가 인덱스 버퍼 연결
	DXUTGetD3D11DeviceContext()->IASetIndexBuffer(m_indexBuff, DXGI_FORMAT_R32_UINT, 0);

	UINT stride = sizeof(VERTEXSTATIC);
	UINT offset = 0;
	//버텍스 버퍼를 IA 에 연결 ( 0번 슬롯 )
	DXUTGetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuff, &stride, &offset);

	//texture 연결
	if (m_textureClone) {
		DXUTGetD3D11DeviceContext()->PSSetShaderResources(0, 1, &m_textureClone);
	}

	// 인덱스 버퍼로 그리기
	DXUTGetD3D11DeviceContext()->DrawIndexed(m_indexCnt, 0, 0);
}


void MeshStatic::Release()
{
	if (m_bCloned)
		return;

	SAFE_RELEASE(m_indexBuff);
	SAFE_RELEASE(m_vertexBuff);
}