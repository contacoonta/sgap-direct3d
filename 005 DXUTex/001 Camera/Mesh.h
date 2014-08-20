#pragma once

#include <DirectXMath.h>

using namespace DirectX;


typedef struct vertexPN
{
	XMFLOAT3 pos;
	XMFLOAT3 nor;

}VERTEXpn, *LPVERTEXpn;


/*
*/
class Mesh
{
	friend class		CompileShader;
	friend class		ObjLoader;

public:
	Mesh();
	~Mesh();

public:
	Mesh*				Clone();
	BOOL				isCloned() const		{ return m_bClone; }

	void				SetWorld(XMMATRIX& mat)	{ m_world = mat;	}
	XMMATRIX			World()					{ return m_world;	}
	
	HRESULT				Initialize();
	void				Update();
	void				Render();
	void				Release();

private:
	ID3D11Buffer*       m_vertexbuffer			= nullptr;
	ID3D11Buffer*       m_indexbuffer			= nullptr;
	UINT				m_indexCnt				= 0;

	XMMATRIX            m_world					= XMMatrixIdentity();
	BOOL				m_bClone				= false;

};

