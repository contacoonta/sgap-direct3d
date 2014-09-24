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

void Camera::AttachTarget(Transform* target)
{
	if (target == nullptr)
		return;
	
	//타겟의 World Matrix 를 받아온다.
	XMMATRIX mtarget	= target->getWorldXM();

	XMVECTOR vup		= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMVECTOR vforward	= XMVectorSet(-0.2f, 2.0f, 2.0f, 0.0f);
	XMVECTOR vbackward	= XMVectorSet(-0.2f, 0.5f, -2.0f, 0.0f);

	// Forward Backward Vector 구하기
	vbackward			= XMVector3TransformCoord(vbackward, mtarget);
	vforward			= XMVector3TransformCoord(vforward, mtarget);

	m_view = XMMatrixLookAtLH(vforward, vbackward, vup);
}

void Camera::computeViewMat()
{
	/*	렌더 파이프라인
		local -> world -> view -> proj -> screen
						| CAMERA       |

		proj	->	view	->	  world		->		local
		   * proj-1		* view-1		* world-1
	*/

	XMVECTOR eye = XMLoadFloat3(&m_eye);
	XMVECTOR look = XMLoadFloat3(&m_lookat);
	XMVECTOR up = XMLoadFloat3(&m_up);
	XMVECTOR offset = XMLoadFloat3(&m_offset);
	
	eye += offset;

	m_view = XMMatrixLookAtLH(eye, look, up);
}

void Camera::computeProjMat()
{
	UINT width = DXUTGetDXGIBackBufferSurfaceDesc()->Width;
	UINT height = DXUTGetDXGIBackBufferSurfaceDesc()->Width;

	m_proj = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / height, 0.01f, 1000.0f);
}