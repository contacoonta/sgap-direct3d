#include "DXUT.h"
#include "SDKmisc.h"

#include "ObjLoader.h"

#include <fstream>

using std::ifstream;



/*
	
*/
HRESULT ObjLoader::ParseFromObj(LPCWSTR wfilename)
{
	ifstream	fin;
	char		dat;
	char		datLine[128] = {};
	XMFLOAT3	f3;
	XMFLOAT2	f2;

	fin.open(wfilename);

	// ���� �б� ����
	if (fin.fail())
		return S_FALSE;

	fin.get(dat);
	while (!fin.eof())
	{
		//# ������ ����
		if (dat == '#')
		{
			fin.getline(datLine, 128);
		}//'#'

		//vertex ���� �б�
		if (dat == 'v')
		{
			fin.get(dat);

			//position ���� �б�
			if (dat == ' ')
			{
				fin >> f3.x >> f3.y >> f3.z;
				posList.push_back(f3);
			}
			
			//texture ���� �б�
			if (dat == 't')
			{
				fin >> f2.x >> f2.y;
				texList.push_back(f2);
			}

			//normal ���� �б�
			if (dat == 'n')
			{
				fin >> f3.x >> f3.y >> f3.z;
				norList.push_back(f3);
			}			
		}//'v'

		/*
			face �� 1���� �����ϴ� -1 �� �ؼ� 0���� ����� �ش�.
		*/
		if (dat == 'f')
		{
			FACE face = {};
			UINT val = 0;

			for (UINT u = 0; u < 3; u++)
			{
				//position
				fin >> val;
				face.pos[u] = --val;

				if (fin.peek() == '/')
				{
					fin.ignore();

					//texture
					if (fin.peek() != '/')
					{
						fin >> val;
						face.tex[u] = --val;
					}

					//normal
					if (fin.peek() == '/')
					{
						fin.ignore();

						fin >> val;
						face.nor[u] = --val;
					}
				}//if('/')

			}//for()

			//�ε����� ������ ����
			faceList.push_back(face);
			
		}//'f'

		fin.get(dat);
	
	}//while()

	fin.close();

	return S_OK;
}


/*
*/
Mesh* ObjLoader::BuildMeshFromFile(LPCWSTR wfilename)
{
	HRESULT hr = S_OK;

	// ������ �޽������� �ִٸ� �����.
	Release();

	/*
		LAYOUT �� �޽� �ʱ�ȭ
	*/
	Mesh* mesh = new Mesh;
	hr = mesh->Initialize();
	if (FAILED(hr))
	{
		delete mesh;
		return nullptr;
	}


	/*
		Obj ���� �о����
	*/
	WCHAR strpathW[256] = {};
	DXUTFindDXSDKMediaFileCch(strpathW, sizeof(strpathW) / sizeof(WCHAR), wfilename);

	hr = ParseFromObj(strpathW);
	if (FAILED(hr))
	{
		delete mesh;
		return nullptr;
	}


	/*
		VERTEX LIST to VERTEX BUFFER
	*/
	vertices = new VERTEXpn[faceList.size()*3];
	{
		UINT u = 0;
		for (auto x : faceList)
		{
			vertices[u].pos = posList[x.pos[0]];
			vertices[u].tex = texList[x.tex[0]];
			vertices[u++].nor = norList[x.nor[0]];
			
			vertices[u].pos = posList[x.pos[1]];
			vertices[u].tex = texList[x.tex[1]];
			vertices[u++].nor = norList[x.nor[1]];
			
			vertices[u].pos = posList[x.pos[2]];
			vertices[u].tex = XMFLOAT2(0.0f, 0.0f);
			vertices[u++].nor = norList[x.nor[2]];
		}

		UINT numVertices = faceList.size() * 3;

		D3D11_BUFFER_DESC buffdesc;
		ZeroMemory(&buffdesc, sizeof(buffdesc));
		buffdesc.Usage = D3D11_USAGE_DEFAULT;
		buffdesc.ByteWidth = sizeof(VERTEXpn)* numVertices;
		buffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffdesc.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = vertices;

		hr = DXUTGetD3D11Device()->CreateBuffer(&buffdesc, &initData, &(mesh->m_vertexbuffer));
		if (FAILED(hr))
		{
			delete mesh;
			return nullptr;
		}

		UINT stride = sizeof(VERTEXpn);
		UINT offset = 0;
		DXUTGetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &(mesh->m_vertexbuffer), &stride, &offset);
	}
	delete[] vertices;
	vertices = nullptr;
	

	/*
		INDEX LIST to INDEX BUFFER
	*/
	indices = new DWORD[faceList.size()*3];
	{
		UINT u = 0;
		for (auto x : faceList)
		{
			indices[u] = u++;
			indices[u] = u++;
			indices[u] = u++;
		}

		mesh->m_indexCnt = faceList.size() * 3;

		D3D11_BUFFER_DESC buffdesc;
		ZeroMemory(&buffdesc, sizeof(buffdesc));
		buffdesc.Usage = D3D11_USAGE_DEFAULT;
		buffdesc.ByteWidth = sizeof(DWORD) * mesh->m_indexCnt;
		buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffdesc.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = indices;
		// �ε��� ���� ����
		hr = DXUTGetD3D11Device()->CreateBuffer(&buffdesc, &initData, &(mesh->m_indexbuffer));
		if (FAILED(hr))
		{
			delete mesh;
			return nullptr;
		}

		// ��ǲ ��������� �ε��� ���� ����
		DXUTGetD3D11DeviceContext()->IASetIndexBuffer(mesh->m_indexbuffer, DXGI_FORMAT_R32_UINT, 0);
		DXUTGetD3D11DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	delete[] indices;
	indices = nullptr;
	

	return mesh;
}


Mesh* ObjLoader::BuildCube()
{
	HRESULT hr = S_OK;

	// ������ �޽������� �ִٸ� �����..
	Release();

	/*
		LAYOUT �� �޽� �ʱ�ȭ
	*/
	Mesh* mesh = new Mesh;
	hr = mesh->Initialize();
	if (FAILED(hr))
	{
		delete mesh;
		return nullptr;
	}


	VERTEXpn cubevertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	};


	WORD cubeindices[] =
	{
		3, 1, 0,
		2, 1, 3,

		6, 4, 5,
		7, 4, 6,

		11, 9, 8,
		10, 9, 11,

		14, 12, 13,
		15, 12, 14,

		19, 17, 16,
		18, 17, 19,

		22, 20, 21,
		23, 20, 22
	};

	/*
		VERTEX LIST �� VERTEX BUFFER �����
	*/
	UINT numVertices = ARRAYSIZE(cubevertices);

	D3D11_BUFFER_DESC buffdesc;
	ZeroMemory(&buffdesc, sizeof(buffdesc));
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.ByteWidth = sizeof(VERTEXpn)* numVertices;
	buffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffdesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = cubevertices;

	hr = DXUTGetD3D11Device()->CreateBuffer(&buffdesc, &initData, &(mesh->m_vertexbuffer));
	if (FAILED(hr))
	{
		delete mesh;
		return nullptr;
	}

	UINT stride = sizeof(VERTEXpn);
	UINT offset = 0;
	DXUTGetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &(mesh->m_vertexbuffer), &stride, &offset);


	/*
		INDEX LIST �� INDEX BUFFER ���� �����
	*/
	mesh->m_indexCnt = ARRAYSIZE(cubeindices);

	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.ByteWidth = sizeof(WORD)* mesh->m_indexCnt;
	buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffdesc.CPUAccessFlags = 0;
	initData.pSysMem = cubeindices;
	// �ε��� ���� ����
	hr = DXUTGetD3D11Device()->CreateBuffer(&buffdesc, &initData, &(mesh->m_indexbuffer));
	if (FAILED(hr))
	{
		delete mesh;
		return nullptr;
	}


	// ��ǲ ��������� �ε��� ���� ����
	DXUTGetD3D11DeviceContext()->IASetIndexBuffer(mesh->m_indexbuffer, DXGI_FORMAT_R16_UINT, 0);
	
	DXUTGetD3D11DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return mesh;
}


/*
*/
void ObjLoader::Release()
{
	posList.clear();
	norList.clear();
	faceList.clear();
}