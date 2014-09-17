#include "DXUT.h"
#include "Transform.h"


Transform::Transform()
{
	XMStoreFloat4x4(&m_world, XMMatrixIdentity());
}

Transform::~Transform()
{
}


void Transform::Move(XMFLOAT3 f3)
{

}