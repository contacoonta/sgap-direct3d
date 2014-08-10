#include "DXUT.h"
#include "MeshLoader.h"

#include <fstream>



using std::ifstream;



MeshLoader::MeshLoader()
{
}

MeshLoader::~MeshLoader()
{
}


void MeshLoader::LoadModelFromFile(LPCWSTR wfilename)
{
	ifstream	fin;
	char		dat;
	XMFLOAT3	f3;

	
	fin.open(wfilename);

	// ���� �б� ����
	if (fin.fail())
		return;
	
	fin.get(dat);
	while (!fin.eof())
	{
		//vertex ���� �б�
		if (dat == 'v')
		{
			fin >> f3.x >> f3.y >> f3.z;
			vertexList.push_back(f3);
		}

		if (dat == 'f')
		{
			fin >> f3.x >> f3.y >> f3.z;
			indexList.push_back(f3);
		}

		fin.get(dat);
	}

	fin.close();
}