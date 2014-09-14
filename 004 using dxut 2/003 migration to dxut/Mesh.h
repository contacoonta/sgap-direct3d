#pragma once

#include <DirectXMath.h>
using namespace DirectX;


typedef struct Vertex
{
	XMFLOAT3 pos;		// 위치 정보
	XMFLOAT3 nor;		// 방향 정보

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
	// 메시의 버텍스 버퍼
	ID3D11Buffer*	m_vertexBuff	= nullptr;
	// 메시의 인덱스 버퍼
	ID3D11Buffer*	m_indexBuff		= nullptr;
	// 인덱스 수
	UINT			m_indexCnt		= 0;

	// 메시의 Transform 정보
	XMMATRIX		m_world		= XMMatrixIdentity();

};
