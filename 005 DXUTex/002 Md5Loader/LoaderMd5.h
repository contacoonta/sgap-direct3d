#pragma once

#include "MeshMd5.h"

class LoaderMd5
{
public:
	Mesh* BuildMeshFromFile(LPCWSTR wfilename);

private:
	BOOL LoadMd5Model(LPCWSTR wfilename, Model3D& MD5Model,
				std::vector<ID3D11ShaderResourceView*>& shaderResourceViewArray,
				std::vector<std::wstring> texFileNameArray);

};