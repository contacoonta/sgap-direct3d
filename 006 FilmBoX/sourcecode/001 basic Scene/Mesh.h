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
	friend class			CompileShader;
	friend class			ObjLoader;

public:
	Mesh();
	~Mesh();

public:
	HRESULT					Initialize();
	void					Update();
	void					Render();
	void					Release();

	XMMATRIX				World()			{ return m_world; }

private:
	ID3D11Buffer*           m_vertexbuffer = nullptr;
	ID3D11Buffer*           m_indexbuffer = nullptr;
	UINT					m_indexCnt = 0;

	XMMATRIX                m_world;
};

