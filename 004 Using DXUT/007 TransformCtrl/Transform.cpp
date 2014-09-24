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
		�� ���͵� ����ȭ
	*/

	// lookat ���͸� �������ͷ� �����.
	L = XMVector3Normalize(L);

	// up ���͸� �������ͷ� �����.
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// right ���͸� �������ͷ� �����.
	R = XMVector3Cross(U, L);

	/*
		��� ���� ä���
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
	// RIGHT ���͸� �������� ȸ�� ��Ʈ������ �����.
	XMMATRIX r = XMMatrixRotationAxis(XMLoadFloat3(&m_right), angle);

	// ȸ���� ��Ʈ���� �������� up ���� �����.
	XMStoreFloat3(&m_up, XMVector3TransformNormal(XMLoadFloat3(&m_up), r));
	// ȸ���� ��Ʈ���� �������� lookat ���� �����.
	XMStoreFloat3(&m_lookat, XMVector3TransformNormal(XMLoadFloat3(&m_lookat), r));
}

void Transform::rotateYaw(float angle)
{
	// Y�� �������� ȸ�� ��Ʈ���� �����.
	XMMATRIX r = XMMatrixRotationY(angle);

	// ȸ���� ��Ʈ���� �������� right ���� �����.
	XMStoreFloat3(&m_right, XMVector3TransformNormal(XMLoadFloat3(&m_right), r));
	// ȸ���� ��Ʈ���� �������� up ���� �����.
	XMStoreFloat3(&m_up, XMVector3TransformNormal(XMLoadFloat3(&m_up), r));
	// ȸ���� ��Ʈ���� �������� lookat ���� �����.
	XMStoreFloat3(&m_lookat, XMVector3TransformNormal(XMLoadFloat3(&m_lookat), r));

	XMStoreFloat3(&m_position, XMVector3TransformNormal(XMLoadFloat3(&m_position), r));
}