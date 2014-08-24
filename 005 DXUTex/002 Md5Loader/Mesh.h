#pragma once

#include <DirectXMath.h>
using namespace DirectX;


typedef struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT3 biTangent;

	// Will not be sent to shader
	int StartWeight = -1;
	int WeightCount = -1;

}VERTEX;


class CompileShader;
class Mesh
{
public:
			Mesh()		{}
	virtual ~Mesh()		{}

public:
	virtual Mesh*		Clone()							{ return nullptr;	}
	BOOL				isCloned() const				{ return m_bClone;	}

	virtual HRESULT		Initialize()					{ return S_OK;		}
	virtual void		Update()						{}
	virtual void		Render(CompileShader* pshader)	{}
	virtual void		Release()						{}

	void				SetWorld(XMMATRIX& mat)			{ m_world = mat;	}
	XMMATRIX			World() const					{ return m_world;	}

protected:
	XMMATRIX            m_world			= XMMatrixIdentity();
	BOOL				m_bClone		= false;
};

