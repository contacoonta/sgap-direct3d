#pragma once

#include <DirectXMath.h>
using namespace DirectX;


typedef struct Vertex
{
	XMFLOAT3 pos;		// ��ġ ����
	XMFLOAT3 nor;		// ���� ����

}VERTEX;


class Mesh
{

public:
	Mesh();
	~Mesh();

public:
	HRESULT		Initialize();
	void		Render(/* CompileShader* pshader */);
	void		Release();

	void		setWorld(XMMATRIX& m)	{ m_world = m;		}
	XMMATRIX	getWorld()				{ return m_world;	}

private:
	// �޽��� ���ؽ� ����
	ID3D11Buffer*	m_vertexBuff	= nullptr;
	// �޽��� �ε��� ����
	ID3D11Buffer*	m_indexBuff		= nullptr;
	// �ε��� ��
	UINT			m_indexCnt		= 0;

	// �޽��� Transform ����
	XMMATRIX		m_world		= XMMatrixIdentity();

};
