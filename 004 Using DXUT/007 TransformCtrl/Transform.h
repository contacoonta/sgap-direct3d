#pragma once

using namespace DirectX;

class Transform
{
public:
	Transform();
	virtual ~Transform();

	virtual void	UpdateMatrix();

	void			setWorld(XMFLOAT4X4 m)		{ m_world = m; }
	XMFLOAT4X4		getWorld()					{ return m_world; }

	//position
	XMVECTOR		getPositionXM() const;
	XMFLOAT3		getPosition() const;
	void			setPositionXM(XMVECTOR v);
	void			setPosition(XMFLOAT3 f3);

	//move delta
	void			moveFoward(float value);
	void			moveStrafe(float value);
	
	//rotation
	void			rotatePitch(float angle);
	void			rotateYaw(float angle);

	//scale	

protected:
	XMFLOAT3		m_position	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT4X4		m_world		= XMFLOAT4X4();

private:
	XMFLOAT3		m_right		= XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3		m_up		= XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3		m_lookat	= XMFLOAT3(0.0f, 0.0f, 1.0f);
};

