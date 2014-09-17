
/*
	Static Mesh ( .obj )
	- 배경 , 장식품 , 고정 오브젝트

	Animated Mesh ( .md5mesh .md5anim )
	- 캐릭터
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
	// 메시의 Transform 정보
	XMFLOAT4		m_color			= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	BOOL			m_bCloned		= FALSE;
};
