
/*
	Static Mesh ( .obj )
	- ��� , ���ǰ , ���� ������Ʈ

	Animated Mash ( .md5mesh .md5anim )
	- ĳ����
*/


#pragma once


using namespace DirectX;

class CompileShader;
class Mesh
{

public:
	Mesh();
	virtual ~Mesh();

public:
	virtual Mesh*		Clone()						{ return nullptr; }

	void				setWorld(XMFLOAT4X4& m)		{ m_world = m; }
	XMFLOAT4X4			getWorld()					{ return m_world; }

public:
	virtual HRESULT		Initialize()							{ return S_OK; }
	virtual void		Render( CompileShader* pshader )		{}
	virtual void		Release()								{}

protected:	
	// �޽��� Transform ����
	XMFLOAT4X4		m_world			= XMFLOAT4X4();
	XMFLOAT4		m_color			= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	BOOL			m_bCloned		= FALSE;
};
