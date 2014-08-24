#include "DXUT.h"
#include "SDKmisc.h"

#include "CompileShader.h"
#include "MeshMd5.h"




MeshMd5::MeshMd5()
{
}

MeshMd5::~MeshMd5()
{
	Release();
}

Mesh* MeshMd5::Clone()
{
	Mesh* pmesh = new MeshMd5;

	/*
		원본의 인덱스 , 버텍스 버퍼를 공유한다.
	*/
	/*pmesh->m_vertexbuffer = m_vertexbuffer;
	pmesh->m_indexbuffer = m_indexbuffer;
	pmesh->m_indexCnt = m_indexCnt;
	pmesh->m_bClone = true;*/

	return pmesh;
}

HRESULT MeshMd5::Initialize()
{
	HRESULT hr = S_OK;
	return hr;
}


void MeshMd5::Update()
{

}

void MeshMd5::Render( CompileShader* pshader )
{
	if (pshader == nullptr)
		return;

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	for (int i = 0; i < m_model.numSubsets; i++)
	{
		//인덱스 버퍼
		DXUTGetD3D11DeviceContext()->IASetIndexBuffer(m_model.subsets[i].indexBuff, DXGI_FORMAT_R32_UINT, 0);
		//버텍스 버퍼
		DXUTGetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_model.subsets[i].vertBuff, &stride, &offset);

		//텍스쳐 설정
		DXUTGetD3D11DeviceContext()->PSSetShaderResources(0, 1, &m_textures[m_model.subsets[i].texArrayIndex]);
		DXUTGetD3D11DeviceContext()->PSSetSamplers(0, 1, &pshader->m_samplerLinear);
		
		//그리기
		DXUTGetD3D11DeviceContext()->DrawIndexed(m_model.subsets[i].indices.size(), 0, 0);
	}
}

void MeshMd5::Release()
{
	if (m_bClone)
		return;

	for (int i = 0; i < m_model.numSubsets; ++i)
	{
		SAFE_RELEASE(m_model.subsets[i].indexBuff);
		SAFE_RELEASE(m_model.subsets[i].vertBuff);
	}

	m_textureNames.clear();
	for (auto x : m_textures) { SAFE_RELEASE(x); }

}