
/*
	Static Mesh ( .obj )
	- ��� , ���ǰ , ���� ������Ʈ

	Animated Mash ( .md5mesh .md5anim )
	- ĳ����
*/


#pragma once


using namespace DirectX;

typedef struct Vertex
{
	XMFLOAT3 pos;		// ��ġ ����
	XMFLOAT3 nor;		// ���� ����
	XMFLOAT3 tex;		// �ؽ��� ��ǥ ����

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
	// �޽��� ���ؽ� ����
	ID3D11Buffer*	m_vertexBuff	= nullptr;
	// �޽��� �ε��� ����
	ID3D11Buffer*	m_indexBuff		= nullptr;
	// �ε��� ��
	UINT			m_indexCnt		= 0;

	// �޽��� Transform ����
	XMFLOAT4X4		m_world			= XMFLOAT4X4();
	XMFLOAT4		m_color			= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	BOOL			m_bCloned		= FALSE;
};
