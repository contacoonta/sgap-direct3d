#include "DXUT.h"

#include <string>
#include <vector>
#include <fbxsdk/scene/geometry/fbxlayer.h>
#include "FbxLoader.h"

using std::string;
using std::wstring;
using std::vector;


static string utf16ToUTF8(const wstring &s)
{
	const int size = ::WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, NULL, 0, 0, NULL);

	vector<char> buf(size);
	::WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, &buf[0], size, 0, NULL);

	return string(&buf[0]);
}

/*
*/
HRESULT FbxLoader::CreateModelFromFile(FbxLoader** pploader, LPCWSTR wfilename, Mesh& mesh)
{
	if (*pploader != nullptr)
		return S_FALSE;

	*pploader = new FbxLoader;
	
	return (*pploader)->LoadFbx(wfilename, mesh);
}

void FbxLoader::Delete(FbxLoader** pploader)
{
	if (*pploader == nullptr)
		return;

	//(*pploader)->Release();

	delete (*pploader);
	*pploader = nullptr;
}


HRESULT FbxLoader::LoadFbx(LPCWSTR wfilename, Mesh& mesh)
{
	FbxManager *manager = FbxManager::Create();

	FbxIOSettings *ioSettings = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ioSettings);

	FbxImporter *importer = FbxImporter::Create(manager, "");
	string filename = utf16ToUTF8(wfilename);
	importer->Initialize(filename.c_str(), -1, manager->GetIOSettings());

	FbxScene *scene = FbxScene::Create(manager, "Scene01");

	importer->Import(scene);
	importer->Destroy();

	m_noderoot = scene->GetRootNode();
	//this->SetModelName(filename);
	//if (rootNode) { this->GetFbxInfo(rootNode); }

	int numKids = m_noderoot->GetChildCount();
	FbxNode *childNode = 0;

	for (int i = 0; i < numKids; i++)
	{
		childNode = m_noderoot->GetChild(i);
		FbxMesh *mesh = childNode->GetMesh();

		if (mesh != NULL)
		{
			/*
				VERTICES
			*/
			int numVerts = mesh->GetControlPointsCount();

			for (int j = 0; j<numVerts; j++)
			{
				FbxVector4 vert = mesh->GetControlPointAt(j);

				/*vertices[numVertices].x = (float)vert.mData[0];
				vertices[numVertices].y = (float)vert.mData[1];
				vertices[numVertices++].z = (float)vert.mData[2];*/				
			}

			/*
				INDICES
			*/
			int numIndices = mesh->GetPolygonVertexCount();
			/*indices = new int[numIndices];
			indices = mesh->GetPolygonVertices();*/


			/*
				NORMALS
			*/
			/*FbxGeometryElementNormal* normalEl = mesh->GetElementNormal();
			if (normalEl)
			{
				numNormals = mesh->GetPolygonCount() * 3;
				normals = new float[numNormals * 3];
				int vertexCounter = 0;
				for (int polyCounter = 0; polyCounter<mesh->GetPolygonCount(); polyCounter++)
				{
					for (int i = 0; i<3; i++)
					{
						FbxVector4 normal = normalEl->GetDirectArray().GetAt(vertexCounter);
						normals[vertexCounter * 3 + 0] = normal[0];
						normals[vertexCounter * 3 + 1] = normal[1];
						normals[vertexCounter * 3 + 2] = normal[2];
						cout << "\n" << normals[vertexCounter * 3 + 0] << " " << normals[vertexCounter * 3 + 1] << " " << normals[vertexCounter * 3 + 2];
						vertexCounter++;
					}
				}
			}*/



		}
	}

	return S_OK;
}

/*
*/
HRESULT FbxLoader::BuildMesh(Mesh& mesh)
{
	HRESULT hr = S_OK;
	

	/*
		VERTEX LIST to VERTEX BUFFER
	*/
	//vertices = new VERTEXpn[faceList.size()];
	//{
	//	UINT u = 0;
	//	for (auto x : posList)
	//	{
	//		vertices[u].pos = x;
	//		vertices[u].nor = norList[u];
	//		u++;
	//	}

	//	UINT numVertices = posList.size();

	//	D3D11_BUFFER_DESC buffdesc;
	//	ZeroMemory(&buffdesc, sizeof(buffdesc));
	//	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	//	buffdesc.ByteWidth = sizeof(VERTEXpn)* numVertices;
	//	buffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//	buffdesc.CPUAccessFlags = 0;
	//	D3D11_SUBRESOURCE_DATA initData;
	//	ZeroMemory(&initData, sizeof(initData));
	//	initData.pSysMem = vertices;

	//	hr = DXUTGetD3D11Device()->CreateBuffer(&buffdesc, &initData, &(mesh.m_vertexbuffer));
	//	delete[] vertices;
	//	if (FAILED(hr))
	//		return hr;

	//	UINT stride = sizeof(VERTEXpn);
	//	UINT offset = 0;
	//	DXUTGetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &(mesh.m_vertexbuffer), &stride, &offset);
	//}
	

	/*
		INDEX LIST to INDEX BUFFER
	*/
	//indices = new DWORD[faceList.size()*3];
	//{
	//	UINT u = 0;
	//	for (auto x : faceList)
	//	{
	//		indices[u++] = x.pos[0];
	//		indices[u++] = x.pos[1];
	//		indices[u++] = x.pos[2];
	//	}

	//	mesh.m_indexCnt = faceList.size() * 3;

	//	D3D11_BUFFER_DESC buffdesc;
	//	ZeroMemory(&buffdesc, sizeof(buffdesc));
	//	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	//	buffdesc.ByteWidth = sizeof(DWORD) * mesh.m_indexCnt;
	//	buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//	buffdesc.CPUAccessFlags = 0;
	//	D3D11_SUBRESOURCE_DATA initData;
	//	ZeroMemory(&initData, sizeof(initData));
	//	initData.pSysMem = indices;
	//	// ÀÎµ¦½º ¹öÆÛ »ý¼º
	//	hr = DXUTGetD3D11Device()->CreateBuffer(&buffdesc, &initData, &(mesh.m_indexbuffer));
	//	delete[] indices;
	//	if (FAILED(hr))
	//		return hr;
	//	// ÀÎÇ² ¾î¼Àºí·¯¿¡ ÀÎµ¦½º ¹öÆÛ ¼³Á¤
	//	DXUTGetD3D11DeviceContext()->IASetIndexBuffer(mesh.m_indexbuffer, DXGI_FORMAT_R32_UINT, 0);
	//	DXUTGetD3D11DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//}
	

	return hr;
}


/*
*/
void FbxLoader::Release()
{
	
}