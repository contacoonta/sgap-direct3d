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
	Release();
}

Mesh* Mesh::Clone()
{
	Mesh* pmesh = new Mesh;

	/*
		원본의 인덱스 , 버텍스 버퍼를 공유한다.
	*/
	pmesh->m_vertexbuffer = m_vertexbuffer;
	pmesh->m_indexbuffer = m_indexbuffer;
	pmesh->m_indexCnt = m_indexCnt;
	pmesh->m_bClone = true;

	return pmesh;
}

HRESULT Mesh::Initialize()
{
	HRESULT hr = S_OK;
	return hr;
}


void Mesh::Update()
{

}

void Mesh::Render()
{
	DXUTGetD3D11DeviceContext()->DrawIndexed(m_indexCnt, 0, 0);
}

void Mesh::Release()
{
	if (m_bClone)
		return;
	
	if (m_vertexbuffer)
	{
		m_vertexbuffer->Release();
		m_vertexbuffer = nullptr;
	}

	if (m_indexbuffer)
	{
		m_indexbuffer->Release();
		m_indexbuffer = nullptr;
	}

}