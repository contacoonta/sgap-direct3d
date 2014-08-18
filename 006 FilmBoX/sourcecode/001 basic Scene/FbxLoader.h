#pragma once

#include <DirectXMath.h>
#include <fbxsdk.h>
#include <vector>
#include "Mesh.h"


class FbxLoader
{
public:
	static HRESULT		CreateModelFromFile(FbxLoader** pploader, LPCWSTR wfilename, Mesh& mesh);
	static void			Delete(FbxLoader** pploader);
	
private:
	HRESULT				LoadFbx(LPCWSTR wfilename, Mesh& mesh);
	HRESULT				BuildMesh(Mesh& mesh);
	void				Release();

private:
	FbxNode*			m_noderoot = nullptr;

};

