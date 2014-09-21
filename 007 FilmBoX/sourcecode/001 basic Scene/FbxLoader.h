#pragma once

#include <DirectXMath.h>
#include <fbxsdk.h>
#include <vector>
#include "Mesh.h"


class FbxLoader
{
private:
	HRESULT				BuildMeshFromFile(LPCWSTR wfilename, Mesh& mesh);	
	void				Release();

private:
	FbxNode*			m_noderoot = nullptr;

	LPVERTEXpn			vertices = nullptr;
	LPDWORD				indices = nullptr;
};

