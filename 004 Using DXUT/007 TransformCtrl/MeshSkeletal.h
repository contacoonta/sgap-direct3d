#pragma once

#include <string>
#include "Mesh.h"

using std::wstring;


typedef struct VertexSkeletal
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;		// 표면의 수직 벡터
	XMFLOAT2 texCoord;		// U V
	
	XMFLOAT3 tangent;		// normal 수직 벡터 u 방향
	XMFLOAT3 biTangent;		// normal 수직 벡터 v 방향

	// Will not be sent to shader
	int StartWeight = -1;
	int WeightCount = -1;

}VERTEXSKELETAL;


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

	std::vector<VERTEXSKELETAL> vertices;
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
class MeshSkeletal : public Mesh
{
	friend class		LoaderMd5;

public:
	MeshSkeletal();
	~MeshSkeletal();

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

