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
			Mesh()		{	m_world	= XMMatrixIdentity();	}
	virtual ~Mesh()		{}

public:
	virtual Mesh*		Clone()							{ return nullptr;	}
	BOOL				isCloned() const				{ return m_bClone;	}
	
	virtual HRESULT		Initialize()					{ return S_OK;		}
	virtual void		Update(float deltaTime)			{}
	virtual void		Render(CompileShader* pshader)	{}
	virtual void		Release()						{}
	
	void				SetWorld(const XMMATRIX& mat)	{ m_world = mat; }
	XMMATRIX			World() const					{ return m_world; }

	void				SetTopology(D3D_PRIMITIVE_TOPOLOGY etype) { m_topology = etype; }

protected:
	XMMATRIX					m_world;
	BOOL						m_bClone	= false;
	D3D11_PRIMITIVE_TOPOLOGY	m_topology	= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

