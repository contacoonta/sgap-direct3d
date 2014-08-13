#pragma once

#include <xnamath.h>
#include <vector>
using std::vector;


typedef struct vertexformat
{
	XMFLOAT3 pos;

}VERTEXFORMAT;

typedef struct constantformat
{
	XMMATRIX mworld;
	XMMATRIX mview;
	XMMATRIX mprojection;

}CONSTANTFORMAT;


class ObjLoader
{
public:
						ObjLoader();
	virtual				~ObjLoader();

public:
	void				LoadModelFromFile(LPCWSTR wfilename);

	void				Initialize();
	void				Update();
	void				Render();
	void				Release();

	void				SetProjection(XMMATRIX& matproj)	{ m_Projection = matproj; }

private:
	HRESULT				ComplieShaderFromFile(WCHAR* wFilename, LPCSTR strEntry, LPCSTR strShaderMdl, ID3DBlob** ppblob);
	
private:
	vector<XMFLOAT3>	vertexList;
	vector<XMFLOAT3>	indexList;

	ID3D11VertexShader*	m_VertexShader		= NULL;
	ID3D11PixelShader*	m_PixelShader		= NULL;
	
	ID3D11InputLayout*	m_VertexLayout		= NULL;
	ID3D11Buffer*		m_VertexBuffer		= NULL;
	ID3D11Buffer*		m_IndexBuffer		= NULL;
	ID3D11Buffer*		m_ConstantBuffer	= NULL;

	XMMATRIX			m_World;
	XMMATRIX			m_View;
	XMMATRIX			m_Projection;

};

