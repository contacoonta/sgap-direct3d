#pragma once

#include <xnamath.h>


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

	HRESULT					BuildCube();
	HRESULT					BuildFromObj(LPCWSTR wfilename);

private:
	CompileShader*			m_compileshader	= nullptr;
	ObjLoader*				m_objloader		= nullptr;

	ID3D11Buffer*           m_vertexbuffer	= nullptr;
	ID3D11Buffer*           m_indexbuffer	= nullptr;
	UINT					m_indexCnt		= 0;

	XMMATRIX                m_world;
	XMMATRIX                m_view;
	XMMATRIX                m_projection;
};

