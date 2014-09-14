#pragma once

#include <vector>
using std::vector;

class LoaderObj
{
public:
	Mesh*		BuildCube();
	Mesh*		BuildMeshFromFile(LPCWSTR wfilename);

private:
	void		Release();

private:

	typedef struct face
	{
		DWORD pos = 0;
		DWORD nor = 0;
		DWORD tex = 0;
	}FACE;

	vector<XMFLOAT3>	posList;	// 버텍스 정보
	vector<XMFLOAT3>	norList;	// 노말(법선) 정보
	vector<XMFLOAT3>	texList;	// 텍스쳐 좌표 정보
	vector<FACE>		faceList;	// 인덱스 정보

};