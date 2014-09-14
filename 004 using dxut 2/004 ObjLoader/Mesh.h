
/*
	Static Mesh ( .obj )
	- 배경 , 장식품 , 고정 오브젝트

	Animated Mash ( .md5mesh .md5anim )
	- 캐릭터
*/


#pragma once


using namespace DirectX;

typedef struct Vertex
{
	XMFLOAT3 pos;		// 위치 정보
	XMFLOAT3 nor;		// 방향 정보
	XMFLOAT3 tex;		// 텍스쳐 좌표 정보

}VERTEX, *LPVERTEX;


class Mesh
{
public:
	friend class LoaderObj;

public:
	Mesh();
	~Mesh();

public:
	Mesh*		Clone();

	void		setWorld(XMFLOAT4X4& m)	{ m_world = m; }
	XMFLOAT4X4	getWorld()				{ return m_world; }

public:
	HRESULT		Initialize();
	void		Render(/* CompileShader* pshader */);
	void		Release();

private:
	// 메시의 버텍스 버퍼
	ID3D11Buffer*	m_vertexBuff	= nullptr;
	// 메시의 인덱스 버퍼
	ID3D11Buffer*	m_indexBuff		= nullptr;
	// 인덱스 수
	UINT			m_indexCnt		= 0;

	// 메시의 Transform 정보
	XMFLOAT4X4		m_world			= XMFLOAT4X4();
	XMFLOAT4		m_color			= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	BOOL			m_bCloned		= FALSE;
};
