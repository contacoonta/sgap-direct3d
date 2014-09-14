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

	vector<XMFLOAT3>	posList;	// ���ؽ� ����
	vector<XMFLOAT3>	norList;	// �븻(����) ����
	vector<XMFLOAT3>	texList;	// �ؽ��� ��ǥ ����
	vector<FACE>		faceList;	// �ε��� ����

};