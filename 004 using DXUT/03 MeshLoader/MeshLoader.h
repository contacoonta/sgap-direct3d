#pragma once

#include <xnamath.h>
#include <vector>
using std::vector;


typedef struct vertexformat
{
	XMFLOAT3 pos;

}VERTEXFORMAT;

class MeshLoader
{
public:
				MeshLoader();
	virtual		~MeshLoader();

public:
	void		LoadModelFromFile(LPCWSTR wfilename);

	void		Initialize();
	void		Render();
	void		Release();

private:
	HRESULT		ComplieShaderFromFile(WCHAR* wFilename, LPCSTR strEntry, LPCSTR strShaderMdl, ID3DBlob** ppblob);
	
private:
	vector<XMFLOAT3>	vertexList;
	vector<XMFLOAT3>	indexList;

	ID3D11VertexShader*	m_VertexShader		= NULL;
	ID3D11PixelShader*	m_PixelShader		= NULL;
	
	ID3D11InputLayout*	m_VertexLayout		= NULL;
	ID3D11Buffer*		m_VertexBuffer		= NULL;
};

