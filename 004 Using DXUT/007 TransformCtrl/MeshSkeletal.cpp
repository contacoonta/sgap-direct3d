#include "DXUT.h"
#include "SDKmisc.h"

#include "CompileShader.h"
#include "MeshSkeletal.h"




MeshSkeletal::MeshSkeletal()
{
}

MeshSkeletal::~MeshSkeletal()
{
	Release();
}

Mesh* MeshSkeletal::Clone()
{
	Mesh* mesh = new MeshSkeletal;
	// 동적 형변환 - 실패시 NULL 반환
	MeshSkeletal* ms = dynamic_cast<MeshSkeletal*>(mesh);
	if (ms == nullptr)
	{
		delete mesh;
		return nullptr;
	}

	/*
		원본의 인덱스 , 버텍스 버퍼를 공유한다.
	*/
	ms->m_model = m_model;
	ms->m_textures = m_textures;
	ms->m_textureNames = m_textureNames;
	
	ms->m_bCloned = true;

	return mesh;
}

HRESULT MeshSkeletal::Initialize()
{
	HRESULT hr = S_OK;
	return hr;
}


void MeshSkeletal::Update()
{

}

void MeshSkeletal::Render( CompileShader* pshader )
{
	if (pshader == nullptr)
		return;

	DXUTGetD3D11DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(VERTEXSKELETAL);
	UINT offset = 0;
	for (int i = 0; i < m_model.numSubsets; i++)
	{
		//인덱스 버퍼
		DXUTGetD3D11DeviceContext()->IASetIndexBuffer(m_model.subsets[i].indexBuff, DXGI_FORMAT_R32_UINT, 0);
		//버텍스 버퍼
		DXUTGetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_model.subsets[i].vertBuff, &stride, &offset);

		//텍스쳐 설정
		DXUTGetD3D11DeviceContext()->PSSetShaderResources(0, 1, &m_textures[m_model.subsets[i].texArrayIndex]);
		
		//그리기
		DXUTGetD3D11DeviceContext()->DrawIndexed(m_model.subsets[i].indices.size(), 0, 0);
	}
}

void MeshSkeletal::Release()
{
	if (m_bCloned)
		return;

	for (int i = 0; i < m_model.numSubsets; ++i)
	{
		SAFE_RELEASE(m_model.subsets[i].indexBuff);
		SAFE_RELEASE(m_model.subsets[i].vertBuff);
	}

	m_textureNames.clear();
	for (auto x : m_textures) { SAFE_RELEASE(x); }

}