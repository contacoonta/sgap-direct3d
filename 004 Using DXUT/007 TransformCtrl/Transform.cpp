#include "DXUT.h"
#include "Transform.h"


Transform::Transform()
{
	XMStoreFloat4x4(&m_world, XMMatrixIdentity());
}

Transform::~Transform()
{
}

void Transform::UpdateMatrix()
{
	XMVECTOR R = XMLoadFloat3(&m_right);
	XMVECTOR U = XMLoadFloat3(&m_up);
	XMVECTOR L = XMLoadFloat3(&m_lookat);
	XMVECTOR P = XMLoadFloat3(&m_position);

	/*
		각 벡터들 직교화
	*/

	// lookat 벡터를 단위벡터로 만든다.
	L = XMVector3Normalize(L);

	// up 벡터를 단위벡터로 만든다.
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// right 벡터를 단위벡터로 만든다.
	R = XMVector3Cross(U, L);

	/*
		행렬 성분 채우기
	*/
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&m_right, R);
	XMStoreFloat3(&m_up, U);
	XMStoreFloat3(&m_lookat, L);


	/*
		1 0 0 0	Side (Right)
		0 1 0 0	Up
		0 0 1 0 Forward
		x y z 1 Position
	*/

	m_world(0, 0) = m_right.x;
	m_world(1, 0) = m_right.y;
	m_world(2, 0) = m_right.z;
	m_world(3, 0) = x;

	m_world(0, 1) = m_up.x;
	m_world(1, 1) = m_up.y;
	m_world(2, 1) = m_up.z;
	m_world(3, 1) = y;

	m_world(0, 2) = m_lookat.x;
	m_world(1, 2) = m_lookat.y;
	m_world(2, 2) = m_lookat.z;
	m_world(3, 2) = z;

	m_world(0, 3) = 0.0f;
	m_world(1, 3) = 0.0f;
	m_world(2, 3) = 0.0f;
	m_world(3, 3) = 1.0f;
}


XMVECTOR Transform::getPositionXM() const
{
	return XMLoadFloat3(&m_position);
}

XMFLOAT3 Transform::getPosition() const
{
	return m_position;
}

void Transform::setPositionXM(XMVECTOR v)
{
	XMStoreFloat3(&m_position, v);
}

void Transform::setPosition(XMFLOAT3 f3)
{
	m_position = f3;
}


void Transform::moveForward(float value)
{
	// m_position += value * lookat
	//XMVECTOR s = XMVectorReplicate(value);
	XMVECTOR s = XMVectorSet(0.0f, 0.0f, value, 0.0f);
	XMVECTOR l = XMLoadFloat3(&m_lookat);
	XMVECTOR p = XMLoadFloat3(&m_position);

	XMStoreFloat3(&m_position, XMVectorMultiplyAdd(s, l, p));
}

void Transform::moveStrafe(float value)
{
	// m_position += value * right
	//XMVECTOR s = XMVectorReplicate(value);
	XMVECTOR s = XMVectorSet(value, 0.0f, 0.0f, 0.0f);
	XMVECTOR r = XMLoadFloat3(&m_right);
	XMVECTOR p = XMLoadFloat3(&m_position);

	XMStoreFloat3(&m_position, XMVectorMultiplyAdd(s, r, p));
}



//rotation
void Transform::rotatePitch(float angle)
{
	// RIGHT 벡터를 기준으로 회전 매트릭스를 만든다.
	XMMATRIX r = XMMatrixRotationAxis(XMLoadFloat3(&m_right), angle);

	// 회전한 매트릭스 기준으로 up 벡터 만든다.
	XMStoreFloat3(&m_up, XMVector3TransformNormal(XMLoadFloat3(&m_up), r));
	// 회전한 매트릭스 기준으로 lookat 벡터 만든다.
	XMStoreFloat3(&m_lookat, XMVector3TransformNormal(XMLoadFloat3(&m_lookat), r));
}

void Transform::rotateYaw(float angle)
{
	// Y축 기준으로 회전 매트릭스 만든다.
	XMMATRIX r = XMMatrixRotationY(angle);

	// 회전한 매트릭스 기준으로 right 벡터 만든다.
	XMStoreFloat3(&m_right, XMVector3TransformNormal(XMLoadFloat3(&m_right), r));
	// 회전한 매트릭스 기준으로 up 벡터 만든다.
	XMStoreFloat3(&m_up, XMVector3TransformNormal(XMLoadFloat3(&m_up), r));
	// 회전한 매트릭스 기준으로 lookat 벡터 만든다.
	XMStoreFloat3(&m_lookat, XMVector3TransformNormal(XMLoadFloat3(&m_lookat), r));

	XMStoreFloat3(&m_position, XMVector3TransformNormal(XMLoadFloat3(&m_position), r));
}