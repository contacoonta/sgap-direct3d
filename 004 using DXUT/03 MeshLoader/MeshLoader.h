#pragma once

#include <xnamath.h>
#include <vector>
using std::vector;

class MeshLoader
{
public:
				MeshLoader();
	virtual		~MeshLoader();

public:
	void		LoadModelFromFile(LPCWSTR wfilename);
	
private:
	vector<XMFLOAT3> vertexList;
	vector<XMFLOAT3> indexList;
};

