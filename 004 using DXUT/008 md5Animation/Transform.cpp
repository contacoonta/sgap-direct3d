#include "DXUT.h"
#include "Transform.h"

#include <math.h>
using namespace DirectX;


Transform::Transform()
{
	XMStoreFloat4x4(&m_world, XMMatrixIdentity());
}

Transform::~Transform()
{
}

void Transform::updateMatrix()
{
	//XMMATRIX mrot = XMMatrixRotationRollPitchYaw(pitchAngle_, yawAngle_, 0.0f);
	XMVECTOR quaternion_ = XMQuaternionRotationRollPitchYaw(pitchAngle_, yawAngle_, 0.0f);
	XMMATRIX mrot = XMMatrixRotationQuaternion(quaternion_);
	
	XMFLOAT4X4 frot;
	XMStoreFloat4x4(&frot, mrot);

	XMFLOAT4 eulerQ = quaternionToEuler(quaternion_);

	XMVECTOR vright		= XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR vup		= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR vforward	= XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	vright				= XMVector3TransformCoord(vright, mrot);
	vup					= XMVector3TransformCoord(vup, mrot);
	vforward			= XMVector3TransformCoord(vforward, mrot);
	XMVECTOR vdelta		= XMLoadFloat3(&posDelta_);
	vdelta				= XMVector3TransformCoord(vdelta, mrot);
	
	// 포지션의 변화값은 초기화 한다.
	posDelta_			= XMFLOAT3(0,0,0);

	// 포지션의 월드 좌표를 계산한다.
	XMVECTOR vpos		= XMLoadFloat3(&m_position);
	vpos				+= vdelta;
	XMStoreFloat3(&m_position, vpos);

	//forward 벡터로 바라볼 곳의 위치를 구한다.
	XMVECTOR vlookat	= vpos + vforward;
	XMStoreFloat3(&m_lookat, vlookat);

	//현재 오브젝트 기준의 View 행렬을 만들고
	XMMATRIX mview = XMMatrixLookAtLH(vpos, vlookat, vup);

	//View 의 역행렬을 이용해 , World 행렬을 구한다.
	XMMATRIX mworld = XMMatrixInverse(nullptr, mview);
	XMStoreFloat4x4(&m_world, mworld);
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

	updateMatrix();
}

void Transform::setPosition(XMFLOAT3 f3)
{
	m_position = f3;

	updateMatrix();
}


void Transform::moveForward(float value)
{
	posDelta_.z -= value;

	updateMatrix();
}

void Transform::moveStrafe(float value)
{
	posDelta_.x -= value;

	updateMatrix();
}



//rotation
void Transform::rotatePitch(float angle)
{
	pitchAngle_ += angle;

	//피치 한계값
	pitchAngle_ = std::max(-XM_PI * 0.5f, pitchAngle_);
	pitchAngle_ = std::min(+XM_PI * 0.5f, pitchAngle_);

	updateMatrix();
}

void Transform::rotateYaw(float angle)
{
	yawAngle_ += angle;

	updateMatrix();
}


//target
void Transform::setTarget(XMFLOAT3 f3)
{
	m_lookat = f3;
		 
	XMVECTOR veye = XMLoadFloat3(&m_position);
	XMVECTOR vlookat = XMLoadFloat3(&f3);
	XMVECTOR vup = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// eye 와 lookat 을 바꿔서 정면이 타겟을 바라보게 만든다.
	XMMATRIX mview = XMMatrixLookAtLH(vlookat, veye, vup);

	//View 의 역행렬을 이용해 , World 행렬을 구한다.
	XMMATRIX mworld = XMMatrixInverse(nullptr, mview);
	XMStoreFloat4x4(&m_world, mworld);

	//Matrix -> Quaternion
	XMVECTOR q = XMQuaternionRotationMatrix(mworld);

	// 쿼터니언에서 오일러 각을 구한다.
	XMFLOAT4 euler = quaternionToEuler(q);
	yawAngle_ = euler.x;
	pitchAngle_ = euler.y;
	
	updateMatrix();
}


XMFLOAT4 Transform::quaternionToEuler(const XMVECTOR q)
{
	XMFLOAT4 euler;
	XMStoreFloat4(&euler, q);	

	double x, y, z, w;

	x = euler.x;
	y = euler.y;
	z = euler.z;
	w = euler.w;
	
	double sqx = x*x;
	double sqy = y*y;
	double sqz = z*z;
	double sqw = w*w;

	//pitch
	euler.y = asinf(2.0f * (w*x - y*z)); // rotation about x-axis 

	//yaw
	euler.x = atan2f(2.0f * (x*z + w*y), (-sqx - sqy + sqz + sqw)); // rotation about y-axis 

	//roll
	//euler.z = atan2f(2.0f * (x*y + w*z), (-sqx + sqy - sqz + sqw)); // rotation about z-axis 
	euler.z = 0.0f;
	euler.w = 0.0f;

	return euler;
}