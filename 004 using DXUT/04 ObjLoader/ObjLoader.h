#pragma once

#include <xnamath.h>
#include <vector>
#include "Mesh.h"

using std::vector;

class ObjLoader
{
public:
	static HRESULT		CreateModelFromFile(ObjLoader** pploader, LPCWSTR wfilename, Mesh& mesh);
	static void			Delete(ObjLoader** pploader);

private:
	HRESULT				ParseFromObj(LPCWSTR wfilename, Mesh& mesh);
	HRESULT				BuildMesh(Mesh& mesh);
	HRESULT				BuildCube(Mesh& mesh);
	void				Release();
	
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

