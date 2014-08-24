#pragma once

#include "Mesh.h"

#include <string>
using std::wstring;


struct SurfaceMaterial
{
	std::wstring matName;
	XMFLOAT4 difColor;
	int texArrayIndex;
	int normMapTexArrayIndex;
	bool hasNormMap;
	bool hasTexture;
	bool transparent;
};

/*
*/
class CompileShader;
class MeshObj : public Mesh
{
	friend class		LoaderObj;

public:
	MeshObj();
	~MeshObj();

public:
	Mesh*				Clone();
	
	HRESULT				Initialize();
	void				Update();
	void				Render( CompileShader* pshader );
	void				Release();

private:
	ID3D11Buffer*			m_vertexBuff		= nullptr;
	ID3D11Buffer*			m_indexBuff			= nullptr;
	INT						m_meshSubsets		= 0;

	std::vector<int>		m_meshSubsetIndexStart;
	std::vector<int>		m_meshSubsetTexture;

	std::vector<SurfaceMaterial>				m_material;
	std::vector<ID3D11ShaderResourceView*>		m_textures;
	std::vector<std::wstring>					m_textureNames;
};

