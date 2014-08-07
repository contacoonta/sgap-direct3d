#include "graphiccamera.h"

SGAP_BEGIN


void iCamera::render()
{
	XMVECTOR up, lookat, position;
	XMMATRIX matRot;

	float yaw, pitch, roll;

	// y-up vector
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// z-foward vector
	lookat = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// 카메라 월드 좌표를 가져온다.
	position = XMLoadFloat3(&m_position);

	// yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch	= XMConvertToRadians(m_rotation.x);	//0.0174532925f;
	yaw		= XMConvertToRadians(m_rotation.y);	
	roll	= XMConvertToRadians(m_rotation.z);
	
	// Create the rotation matrix from the yaw, pitch, and roll values.
	matRot	= XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	// lookat 과 up vector 를 원점에서 matRot 만큼 적용된 vector로 얻는다.
	lookat	= XMVector3TransformCoord(lookat, matRot);
	up		= XMVector3TransformCoord(up, matRot);

	// Translate the rotated camera position to the location of the viewer.
	// 회전 적용된 카메라 lookat 을 position 만큼 이동 시킨다.
	lookat = position + lookat;

	// Finally create the view matrix from the three updated vectors.
	// 위의 세가지 갱신된 vector들을 이용해 view matrix 를 완성한다.
	m_viewMatrix = XMMatrixLookAtLH(position, lookat, up);

}


bool graphicCamera::Create(iCamera** ppcamera)
{
	if (*ppcamera != nullptr)
		return false;

	*ppcamera = new graphicCamera;
	(*ppcamera)->initialize();

	return true;
}

void graphicCamera::Destroy(iCamera** ppcamera)
{
	if (*ppcamera)
	{
		(*ppcamera)->release();

		delete *ppcamera;
		*ppcamera = nullptr;
	}
}


graphicCamera::graphicCamera()
{
}

graphicCamera::graphicCamera(const graphicCamera&)
{
}

graphicCamera::~graphicCamera()
{
}


void graphicCamera::initialize()
{
	
}

void graphicCamera::release()
{

}




SGAP_END