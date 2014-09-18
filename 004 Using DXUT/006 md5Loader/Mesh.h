
/*
	Static Mesh ( .obj )
	- 배경 , 장식품 , 고정 오브젝트

	Animated Mash ( .md5mesh .md5anim )
	- 캐릭터
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
	// 메시의 Transform 정보
	XMFLOAT4X4		m_world			= XMFLOAT4X4();
	XMFLOAT4		m_color			= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	BOOL			m_bCloned		= FALSE;
};
