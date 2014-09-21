#include "DXUT.h"
#include "SDKmisc.h"
#include "Mesh.h"
#include "MeshStatic.h"

#include "ResourceManager.h"
#include "LoaderObj.h"

#include <fstream>
using std::vector;
using std::ifstream;


Mesh* LoaderObj::BuildMeshFromFile(LPCWSTR wfilename, LPCWSTR wtexname)
{
	Release();

	HRESULT		hr	= S_OK;
	CHAR		dat	= 0;
	CHAR		datLine[MAX_PATH] = {};
	XMFLOAT3	f3;
	DWORD		dwdat = 0;

	Mesh* mesh = new MeshStatic;
	MeshStatic* ms = dynamic_cast<MeshStatic*>(mesh);

	ifstream fin;

	WCHAR strpathW[256] = {};
	DXUTFindDXSDKMediaFileCch(strpathW, sizeof(strpathW) / sizeof(WCHAR), wfilename);
	
	fin.open(strpathW);
	
	// 파일 읽기 실패
	if (fin.fail())
	{
		std::wstring message = L"Could not open: ";
		message += wfilename;

		MessageBox(0, message.c_str(), L"Error", MB_OK);

		SAFE_DELETE(mesh);
		return nullptr;
	}

	// 일단 하나씩 읽기
	fin.get(dat);
	// 
	while ( fin.eof() == false )
	{
		// # 라인 무시
		if (dat == '#')
		{
			fin.getline(datLine, MAX_PATH);
		}

		if (dat == 'v')
		{
			// 빈칸 하나 건너띄고 읽기
			fin.get(dat);

			// position 정보
			if (dat == ' ')
			{
				fin >> f3.x >> f3.y >> f3.z;
				posList.push_back(f3);
			}

			// normal 정보
			if (dat == 'n')
			{
				fin >> f3.x >> f3.y >> f3.z;
				norList.push_back(f3);
			}

			// texcoord 정보
			if (dat == 't')
			{
				fin >> f3.x >> f3.y >> f3.z;
				texList.push_back(f3);
			}
		}

		// face 정보
		if (dat == 'f')
		{

			for (int n = 0; n < 3; n++)
			{

				FACE f = {};
				// position index
				fin >> f.pos;
				f.pos--;

				if (fin.peek() == '/')
				{
					fin.ignore();

					if (fin.peek() != '/')
					{
						fin >> f.tex;
						// texture index
						f.tex--;
					}

					if (fin.peek() == '/')
					{
						fin.ignore();
						// normal index
						fin >> f.nor;
						f.nor--;
					}
				}
				faceList.push_back(f);

			}//for ( ... )

		}// if ( dat ... )

		fin.get(dat);
	}
	
	fin.close();

	//	fin.fail();
	//	fin.eof();
	//	fin.ignore();
	//	fin.close();

	/*
		버텍스 버퍼 만들기
	*/
	VERTEXSTATIC* vertices = new VERTEXSTATIC[faceList.size()];
	{
		// range based for
		UINT u = 0;
		for (auto x : faceList)
		{
			vertices[u].pos = posList[x.pos];
			vertices[u].nor = norList[x.nor];
			vertices[u].tex = texList[x.tex];
			u++;
		}
		UINT numVertices = faceList.size();

		D3D11_BUFFER_DESC buffdesc;
		ZeroMemory(&buffdesc, sizeof(buffdesc));
		buffdesc.Usage = D3D11_USAGE_DEFAULT;
		buffdesc.ByteWidth = sizeof(VERTEXSTATIC)* numVertices;
		buffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffdesc.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = vertices ;
		//버텍스 버퍼 생성
		hr = DXUTGetD3D11Device()->CreateBuffer(&buffdesc, &initData, &(ms->m_vertexBuff));

	}
	SAFE_DELETE_ARRAY(vertices);


	
	/*
		인덱스 버퍼 만들기
	*/
	DWORD* indices = new DWORD[faceList.size()];
	{
		// range based for
		UINT u = 0;
		for (auto x : faceList)
		{
			indices[u] = u;
			u++;
		}
		ms->m_indexCnt = faceList.size();

		D3D11_BUFFER_DESC buffdesc;
		ZeroMemory(&buffdesc, sizeof(buffdesc));
		buffdesc.Usage = D3D11_USAGE_DEFAULT;
		buffdesc.ByteWidth = sizeof(DWORD)* ms->m_indexCnt;
		buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffdesc.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = indices;

		//인덱스 버퍼 생성
		hr = DXUTGetD3D11Device()->CreateBuffer(&buffdesc, &initData, &(ms->m_indexBuff));

	}
	SAFE_DELETE_ARRAY(indices);



	/*
		Texture load
	*/
	// 텍스쳐가 있으면 , 불러오기
	if (wtexname)
	{
		//hr = CreateTextureFromFile(mesh, wtexname);
		ms->m_textureClone = RESMGR.AddTextureFromFile(wtexname);
	}

	return mesh;
}

Mesh* LoaderObj::BuildCube()
{
	Release();

	HRESULT hr = S_OK;	

	Mesh* mesh = new MeshStatic;
	MeshStatic* ms = dynamic_cast<MeshStatic*>(mesh);

	/*
		Vertex List -> Vertex Buffer 로 만들기
	*/
	VERTEXSTATIC vertices[] =
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

	UINT numVertices = ARRAYSIZE(vertices);

	D3D11_BUFFER_DESC buffdesc;
	ZeroMemory(&buffdesc, sizeof(buffdesc));
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.ByteWidth = sizeof(VERTEXSTATIC)* numVertices;
	buffdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffdesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertices;
	//버텍스 버퍼 생성
	hr = DXUTGetD3D11Device()->CreateBuffer(&buffdesc, &initData, &(ms->m_vertexBuff));

	UINT stride = sizeof(VERTEXSTATIC);
	UINT offset = 0;
	//버텍스 버퍼를 IA 에 연결
	DXUTGetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &(ms->m_vertexBuff), &stride, &offset);


	/*
		Index List -> Index Buffer 로 만들기
	*/
	DWORD indices[] =
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

	ms->m_indexCnt = ARRAYSIZE(indices);

	
	buffdesc.Usage = D3D11_USAGE_DEFAULT;
	buffdesc.ByteWidth = sizeof(DWORD)* ms->m_indexCnt;
	buffdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffdesc.CPUAccessFlags = 0;
	initData.pSysMem = indices;

	//인덱스 버퍼 생성
	hr = DXUTGetD3D11Device()->CreateBuffer(&buffdesc, &initData, &(ms->m_indexBuff));

	//IA 에다가 인덱스 버퍼 연결
	DXUTGetD3D11DeviceContext()->IASetIndexBuffer(ms->m_indexBuff, DXGI_FORMAT_R32_UINT, 0);


	return mesh;
}


void LoaderObj::Release()
{
	posList.clear();
	norList.clear();
	texList.clear();
	faceList.clear();
}

/*
*/
//HRESULT LoaderObj::CreateTextureFromFile(Mesh* mesh, LPCWSTR wfilename)
//{
//	HRESULT hr = S_OK;
//
//	// 동적 형변환 - 실패시 NULL 반환
//	MeshStatic* ms = dynamic_cast<MeshStatic*>(mesh);
//	if (ms == nullptr)
//	{
//		return S_FALSE;
//	}
//
//
//	// texture 를 찾았다, 
//	ID3D11ShaderResourceView* psrv = RESMGR.GetTexture(wfilename);
//	if (psrv)
//	{
//		ms->m_textureClone = psrv;
//		return S_OK;
//	}
//
//	// texture 가 없으면 , 새로 불러오기
//
//	WCHAR strpathW[256] = {};
//	DXUTFindDXSDKMediaFileCch(strpathW, sizeof(strpathW) / sizeof(WCHAR), wfilename);
//
//	//텍스쳐 파일 불러오기 ( .DDS )	
//	hr = DXUTCreateShaderResourceViewFromFile(DXUTGetD3D11Device(), strpathW, &psrv);
//	if (FAILED(hr))
//		return hr;
//
//	// 불러온 texture 를 리소스 매니져에 연결
//	RESMGR.AddTexture(wfilename, psrv);
//
//	// 
//	ms->m_textureClone = psrv;
//
//	return S_OK;
//}
