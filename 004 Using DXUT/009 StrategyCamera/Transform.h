#pragma once

using namespace DirectX;

class Transform
{
public:
	Transform();
	virtual ~Transform();

	void			setWorld(XMFLOAT4X4 m)		{ m_world = m; }
	XMFLOAT4X4		getWorld() const			{ return m_world; }
	XMMATRIX		getWorldXM() const			{ return XMLoadFloat4x4(&m_world);	}

	//position
	XMVECTOR		getPositionXM() const;
	XMFLOAT3		getPosition() const;
	void			setPositionXM(XMVECTOR v);
	void			setPosition(XMFLOAT3 f3);

	//move delta
	void			moveForward(float value);
	void			moveStrafe(float value);
	
	//rotation
	void			rotatePitch(float angle);
	void			rotateYaw(float angle);

	//target
	void			setTarget(XMFLOAT3 f3);
	float			lengthTarget(XMFLOAT3 f3);

	//scale	

private:
	void		updateMatrix();
	XMFLOAT4	quaternionToEuler(const XMVECTOR q);

protected:
	XMFLOAT3	m_lookat	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3	m_position	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT4X4	m_world;

private:
	XMFLOAT3	posDelta_	= XMFLOAT3(0, 0, 0);		// 상대적 위치 변화값
	FLOAT		pitchAngle_ = 0.0f;						// pitch angle ( Euler Angle )
	FLOAT		yawAngle_	= 0.0f;						// yaw angle ( Euler Angle )
	XMVECTOR	quaternion_ = XMVectorSet(0, 0, 0, 0);	// 정확한 회전을 위한 Quaternion 저장
};

