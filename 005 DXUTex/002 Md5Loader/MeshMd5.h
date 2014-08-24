#pragma once

#include "Mesh.h"

#include <string>
using std::wstring;





typedef struct Joint
{
	wstring name;
	int parentID;

	XMFLOAT3 pos;
	XMFLOAT4 orientation;

}JOINT;


typedef struct Weight
{
	int jointID;
	float bias;
	XMFLOAT3 pos;

}WEIGHT;


typedef struct ModelSubset
{
	int texArrayIndex;
	int numTriangles;

	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;
	std::vector<Weight> weights;

	std::vector<XMFLOAT3> positions;

	ID3D11Buffer* vertBuff;
	ID3D11Buffer* indexBuff;

}MODELSUBSET;


typedef struct Model3D
{
	int numSubsets;
	int numJoints;

	std::vector<Joint> joints;
	std::vector<ModelSubset> subsets;

}MODEL3D, *LPMODEL3D;


/*
*/
class CompileShader;
class MeshMd5 : public Mesh
{
	friend class		LoaderMd5;

public:
	MeshMd5();
	~MeshMd5();

public:
	Mesh*				Clone();
	
	HRESULT				Initialize();
	void				Update();
	void				Render( CompileShader* pshader );
	void				Release();

private:
	MODEL3D										m_model;
	std::vector<ID3D11ShaderResourceView*>		m_textures;
	std::vector<std::wstring>					m_textureNames;
};

