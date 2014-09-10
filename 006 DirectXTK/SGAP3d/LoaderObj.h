#pragma once

#include "MeshObj.h"


class LoaderObj
{
public:
	Mesh*				BuildMeshFromFile(LPCWSTR wfilename);

private:
	
	bool LoadObjModel(LPCWSTR wfilename, ID3D11Buffer** vertBuff, ID3D11Buffer** indexBuff,
					std::vector<int>& subsetIndexStart, std::vector<int>& subsetMaterialArray, std::vector<SurfaceMaterial>& material, 
					int& subsetCount, 
					std::vector<ID3D11ShaderResourceView*>& shaderResourceViewArray,
					std::vector<std::wstring> texFileNameArray,
					std::vector<XMFLOAT3>& vertexArray, std::vector<DWORD>& IndexArray );
	
};

