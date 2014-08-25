#include "DXUT.h"
#include "SDKmisc.h"

#include "CompileShader.h"
#include "MeshObj.h"




MeshObj::MeshObj()
{
}

MeshObj::~MeshObj()
{
	Release();
}

Mesh* MeshObj::Clone()
{
	Mesh* pmesh = new MeshObj;

	/*
		������ �ε��� , ���ؽ� ���۸� �����Ѵ�.
	*/
	/*pmesh->m_vertexbuffer = m_vertexbuffer;
	pmesh->m_indexbuffer = m_indexbuffer;
	pmesh->m_indexCnt = m_indexCnt;
	pmesh->m_bClone = true;*/

	return pmesh;
}

HRESULT MeshObj::Initialize()
{
	HRESULT hr = S_OK;
	return hr;
}


void MeshObj::Update(float deltaTime)
{

}

void MeshObj::Render( CompileShader* pshader )
{
	if (pshader == nullptr)
		return;

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;

	for (int i = 0; i < m_meshSubsets; ++i)
	{
		DXUTGetD3D11DeviceContext()->IASetIndexBuffer(m_indexBuff, DXGI_FORMAT_R32_UINT, 0);
		DXUTGetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuff, &stride, &offset);

		if (m_material[m_meshSubsetTexture[i]].hasTexture)
			DXUTGetD3D11DeviceContext()->PSSetShaderResources(0, 1, &m_textures[m_material[m_meshSubsetTexture[i]].texArrayIndex]);
		if (m_material[m_meshSubsetTexture[i]].hasNormMap)
			DXUTGetD3D11DeviceContext()->PSSetShaderResources(1, 1, &m_textures[m_material[m_meshSubsetTexture[i]].normMapTexArrayIndex]);
		
		DXUTGetD3D11DeviceContext()->PSSetSamplers(0, 1, &pshader->m_samplerLinear);

		int indexStart = m_meshSubsetIndexStart[i];
		int indexDrawAmount = m_meshSubsetIndexStart[i + 1] - m_meshSubsetIndexStart[i];
		if (!m_material[m_meshSubsetTexture[i]].transparent)
			DXUTGetD3D11DeviceContext()->DrawIndexed(indexDrawAmount, indexStart, 0);
	}

}

void MeshObj::Release()
{
	if (m_bClone)
		return;

	SAFE_RELEASE(m_vertexBuff);
	SAFE_RELEASE(m_indexBuff);
	m_meshSubsets = 0;

	m_meshSubsetIndexStart.clear();
	m_meshSubsetTexture.clear();
	m_material.clear();

	m_textureNames.clear();
	for (auto x : m_textures) { SAFE_RELEASE(x); }

}