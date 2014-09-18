
/*
	Static Mesh ( .obj )
	- ��� , ���ǰ , ���� ������Ʈ

	Animated Mesh ( .md5mesh .md5anim )
	- ĳ����
*/


#pragma once

#include "Transform.h"

using namespace DirectX;

class CompileShader;
class Mesh : public Transform
{

public:
	Mesh();
	virtual ~Mesh();

public:
	virtual Mesh*		Clone()						{ return nullptr; }

public:
	virtual HRESULT		Initialize()							{ return S_OK; }
	virtual void		Render( CompileShader* pshader )		{}
	virtual void		Release()								{}

protected:	
	// �޽��� Transform ����
	XMFLOAT4		m_color			= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	BOOL			m_bCloned		= FALSE;
};
