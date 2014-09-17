#pragma once

using namespace DirectX;

class Transform
{
public:
	Transform();
	virtual ~Transform();

	void			setWorld(XMFLOAT4X4& m)		{ m_world = m; }
	XMFLOAT4X4		getWorld()					{ return m_world; }

	void			Move(XMFLOAT3 f3);

protected:
	XMFLOAT4X4		m_world = XMFLOAT4X4();
};

