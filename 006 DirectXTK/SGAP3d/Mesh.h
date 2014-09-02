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
			Mesh()		{	XMStoreFloat4x4(&m_world, XMMatrixIdentity());	}
	virtual ~Mesh()		{}

public:
	virtual Mesh*		Clone()							{ return nullptr;	}
	BOOL				isCloned() const				{ return m_bClone;	}

	virtual HRESULT		Initialize()					{ return S_OK;		}
	virtual void		Update(float deltaTime)			{}
	virtual void		Render(CompileShader* pshader)	{}
	virtual void		Release()						{}

	void				SetWorld(XMFLOAT4X4 mat)		{ m_world = mat;	}
	XMFLOAT4X4			World() const					{ return m_world;	}

	int m_ani = 0;

protected:
	XMFLOAT4X4          m_world;
	BOOL				m_bClone		= false;
};

