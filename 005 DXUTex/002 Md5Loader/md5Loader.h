#pragma once

#include <DirectXMath.h>
#include <string>

using std::wstring;
using namespace DirectX;


//D3D11_INPUT_ELEMENT_DESC layout[] =
//{
//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
//};
//UINT numElements = ARRAYSIZE(layout);


typedef struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT3 biTangent;

	// Will not be sent to shader
	int StartWeight	= -1;
	int WeightCount	= -1;

}VERTEX;


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

}MODEL3D;


class md5Loader
{

public:
	md5Loader();
	~md5Loader();

public:
	BOOL LoadMd5Model(LPCWSTR wfilename, Model3D& MD5Model,
				std::vector<ID3D11ShaderResourceView*>& shaderResourceViewArray,
				std::vector<std::wstring> texFileNameArray);

};