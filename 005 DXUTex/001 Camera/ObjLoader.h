#pragma once

#include <DirectXMath.h>
#include <vector>
#include <map>

#include "Mesh.h"

using namespace DirectX;
using std::vector;
using std::map;


class ObjLoader
{
public:
	Mesh*				BuildMeshFromFile(LPCWSTR wfilename);
	Mesh*				BuildCube();
	
	void				Release();

private:
	HRESULT				ParseFromObj(LPCWSTR wfilename);

private:
	typedef struct face
	{
		FLOAT pos[3];
		FLOAT tex[3];
		FLOAT nor[3];
	}FACE;

	vector<XMFLOAT3>	posList;
	vector<XMFLOAT2>	texList;
	vector<XMFLOAT3>	norList;
	vector<FACE>		faceList;

	LPVERTEXpn			vertices = nullptr;
	LPDWORD				indices = nullptr;
};

