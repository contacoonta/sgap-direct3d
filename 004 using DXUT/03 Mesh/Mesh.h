#pragma once

#include <d3d11.h>
#include <xnamath.h>

typedef struct SimpleVertex
{
	XMFLOAT3 pos;

}SIMPLEVERTEX;


/*
*/
class CompileShader;

class Mesh
{

public:
	Mesh();
	~Mesh();

public:
	HRESULT Initialize();
	void Render();
	void Release();

private:
	CompileShader*			m_compileshader	= nullptr;

	ID3D11Buffer*           m_vertexbuffer	= nullptr;
};

