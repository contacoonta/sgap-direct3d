#include "DXUT.h"
#include "Transform.h"
#include "Camera.h"

using namespace DirectX;

Camera::Camera()
{
}

Camera::~Camera()
{
}

/*
*/
void Camera::setEye(XMFLOAT3 eye)
{
	m_eye = eye;

	computeViewMat();
}

void Camera::setOffset(XMFLOAT3 offset)
{
	m_offset = offset;

	computeViewMat();
}

void Camera::setLookat(XMFLOAT3 lookat)
{
	m_lookat = lookat;

	computeViewMat();
}

void Camera::computeViewMat()
{
	/*	���� ����������
		local -> world -> view -> proj -> screen
						| CAMERA       |

		proj	->	view	->	  world		->		local
		   * proj-1		* view-1		* world-1
	*/

	m_eye.x += m_offset.x;
	m_eye.y += m_offset.y;
	m_eye.z += m_offset.z;

	XMVECTOR e = XMLoadFloat3(&m_eye);
	XMVECTOR l = XMLoadFloat3(&m_lookat);
	XMVECTOR up = XMLoadFloat3(&m_up);
	
	m_view = XMMatrixLookAtLH(e, l, up);
}

void Camera::computeProjMat()
{
	UINT width = DXUTGetDXGIBackBufferSurfaceDesc()->Width;
	UINT height = DXUTGetDXGIBackBufferSurfaceDesc()->Width;

	m_proj = XMMatrixPerspectiveFovLH(XM_PIDIV2, (FLOAT)width / (FLOAT)height, 0.01f, 1000.0f);
}

// ��ü�� ������ų Ÿ��
void Camera::attachTarget(Transform* target)
{
	if (target == nullptr)
		return;
	
	//Ÿ���� ���� matrix
	XMMATRIX mtarget = target->getWorldXM();

	//Up vector - Ÿ�� ������ ���� ��ǥ 
	XMVECTOR vup = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR vforward = XMVectorSet(-0.2f, 2.0f, 2.0f, 0.0f);
	XMVECTOR vbackward = XMVectorSet(-0.2f, 0.5f, -2.0f, 0.0f);

	//Ÿ�� ����� �̿��ؼ� ���� ��ǥ ��ȯ
	vforward = XMVector3TransformCoord(vforward, mtarget);
	vbackward = XMVector3TransformCoord(vbackward, mtarget);

	// ī�޶��� View matrix ���ϱ�
	m_view = XMMatrixLookAtLH(vforward, vbackward, vup);
}