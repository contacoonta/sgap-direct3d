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

	// 파일 읽기 실패
	if (fin.fail())
		return S_FALSE;

	fin.get(dat);
	while (!fin.eof())
	{
		//# 라인은 무시
		if (dat == '#')
		{
			fin.getline(datLine, 128);
		}//'#'

		//vertex 정보 읽기
		if (dat == 'v')
		{
			fin.get(dat);

			//position 정보 읽기
			if (dat == ' ')
			{
				fin >> f3.x >> f3.y >> f3.z;
				posList.push_back(f3);
			}
			
			/*if (dat == 't')
			{
				fin >> f2.x >> f2.y;
			}*/

			//normal 정보 읽기
			if (dat == 'n')
			{
				fin >> f3.x >> f3.y >> f3.z;
				norList.push_back(f3);
			}			
		}//'v'

		/*
			face 는 1부터 시작하니 -1 을 해서 0으로 만들어 준다.
		*/
		if (dat == 'f')
		{
			FACE face = {};
			FLOAT fval;

			for (UINT u = 0; u < 3; u++)
			{
				fin >> fval;
				face.pos[u] = --fval;

				if (fin.peek() == '/')
				{
					fin.ignore();

					if (fin.peek() != '/')
					{
						fin >> fval;
					}

					if (fin.peek() == '/')
					{
						fin.ignore();

						fin >> fval;
						face.nor[u] = --fval;
					}
				}//if('/')

			}//for()

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

	// 기존의 메시정보가 있다면 지운다.
	Release();

	/*
		LAYOUT 등 메시 초기화
	*/
	Mesh* mesh = new Mesh;
	hr = mesh->Initialize();
	if (FAILED(hr))
	{
		delete mesh;
		return nullptr;
	}


	/*
		Obj 파일 읽어오기
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
	vertices = new VERTEXpn[faceList.size()];
	{
		UINT u = 0;
		for (auto x : posList)
		{
			vertices[u].pos = x;
			vertices[u].nor = norList[u];
			u++;
		}

		UINT numVertices = posList.size();

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
			indices[u++] = x.pos[0];
			indices[u++] = x.pos[1];
			indices[u++] = x.pos[2];
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
		// 인덱스 버퍼 생성
		hr = DXUTGetD3D11Device()->CreateBuffer(&buffdesc, &initData, &(mesh->m_indexbuffer));
		if (FAILED(hr))
		{
			delete mesh;
			return nullptr;
		}

		// 인풋 어셈블러에 인덱스 버퍼 설정
		DXUTGetD3D11DeviceContext()->IASetIndexBuffer(mesh->m_indexbuffer, DXGI_FORMAT_R32_UINT, 0);
		DXUTGetD3D11DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	delete[] indices;
	indices = nullptr;
	

	return mesh;
}


HRESULT ObjLoader::BuildCube(Mesh* mesh)
{
	HRESULT hr = S_OK;

	// 기존의 메시정보가 있다면 지운다..
	Release();
	mesh->Release();

	/*
		LAYOUT 등 메시 초기화
	*/
	hr = mesh->Initialize();
	if (FAILED(hr))
		return hr;


	VERTEXpn cubevertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },

		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
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
		VERTEX LIST 로 VERTEX BUFFER 만들기
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
		return hr;

	UINT stride = sizeof(VERTEXpn);
	UINT offset = 0;
	DXUTGetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &(mesh->m_vertexbuffer), &stride, &offset);


	/*
		INDEX LIST 로 INDEX BUFFER 버퍼 만들기
	*/
	mesh->m_indexCnt = ARRAYSIZE(cubeindices);

	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.ByteWidth = sizeof(WORD)* mesh->m_indexCnt;
	buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffdesc.CPUAccessFlags = 0;
	initData.pSysMem = cubeindices;
	// 인덱스 버퍼 생성
	hr = DXUTGetD3D11Device()->CreateBuffer(&buffdesc, &initData, &(mesh->m_indexbuffer));
	if (FAILED(hr))
		return hr;
	// 인풋 어셈블러에 인덱스 버퍼 설정
	DXUTGetD3D11DeviceContext()->IASetIndexBuffer(mesh->m_indexbuffer, DXGI_FORMAT_R16_UINT, 0);
	
	DXUTGetD3D11DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


/*
*/
void ObjLoader::Release()
{
	posList.clear();
	norList.clear();
	faceList.clear();
}