#pragma once

#include <xnamath.h>

typedef struct SimpleVertex
{
	XMFLOAT3 pos;
	XMFLOAT3 nor;

}SIMPLEVERTEX;


/*
*/

class Mesh
{
	friend class CompileShader;

public:
	Mesh();
	~Mesh();

public:
	HRESULT Initialize();
	void Update();
	void Render();
	void Release();

private:
	CompileShader*			m_compileshader	= nullptr;

	ID3D11Buffer*           m_vertexbuffer	= nullptr;
	ID3D11Buffer*           m_indexbuffer = nullptr;

	XMMATRIX                m_world;
	XMMATRIX                m_view;
	XMMATRIX                m_projection;
};

