#pragma once

#include <d3d11.h>
#include <xnamath.h>

typedef struct SimpleVertex
{
	XMFLOAT3 pos;
	XMFLOAT3 nor;

}SIMPLEVERTEX;

typedef struct ConstantBuffer
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;

	XMFLOAT4 litDir;		// ������ ����
	XMFLOAT4 litCol;		// ������ ��

	XMFLOAT4 colOutput;		// ���� �ȼ� ����

}CONSTANTBUFFER;


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
	void Update();
	void Render();
	void Release();

private:
	CompileShader*			m_compileshader	= nullptr;

	ID3D11Buffer*           m_vertexbuffer	= nullptr;
	ID3D11Buffer*           m_indexbuffer = nullptr;
	ID3D11Buffer*           m_constantbuffer = nullptr;

	XMMATRIX                m_world;
	XMMATRIX                m_view;
	XMMATRIX                m_projection;
};

