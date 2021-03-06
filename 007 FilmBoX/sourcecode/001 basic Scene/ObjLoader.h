#pragma once

#include <DirectXMath.h>
#include <vector>
#include "Mesh.h"

using namespace DirectX;
using std::vector;

class ObjLoader
{
public:
	HRESULT				BuildMeshFromFile(LPCWSTR wfilename, Mesh& mesh);
	HRESULT				BuildCube(Mesh& mesh);

	void				Release();

private:
	HRESULT				ParseFromObj(LPCWSTR wfilename);

private:
	typedef struct face
	{
		FLOAT pos[3];
		FLOAT nor[3];
	}FACE;

	vector<XMFLOAT3>	posList;
	vector<XMFLOAT3>	norList;
	vector<FACE>		faceList;

	LPVERTEXpn			vertices = nullptr;
	LPDWORD				indices = nullptr;
};

