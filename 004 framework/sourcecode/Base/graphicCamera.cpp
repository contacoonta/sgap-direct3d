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

	// ī�޶� ���� ��ǥ�� �����´�.
	position = XMLoadFloat3(&m_position);

	// yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch	= XMConvertToRadians(m_rotation.x);	//0.0174532925f;
	yaw		= XMConvertToRadians(m_rotation.y);	
	roll	= XMConvertToRadians(m_rotation.z);
	
	// Create the rotation matrix from the yaw, pitch, and roll values.
	matRot	= XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	// lookat �� up vector �� �������� matRot ��ŭ ����� vector�� ��´�.
	lookat	= XMVector3TransformCoord(lookat, matRot);
	up		= XMVector3TransformCoord(up, matRot);

	// Translate the rotated camera position to the location of the viewer.
	// ȸ�� ����� ī�޶� lookat �� position ��ŭ �̵� ��Ų��.
	lookat = position + lookat;

	// Finally create the view matrix from the three updated vectors.
	// ���� ������ ���ŵ� vector���� �̿��� view matrix �� �ϼ��Ѵ�.
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