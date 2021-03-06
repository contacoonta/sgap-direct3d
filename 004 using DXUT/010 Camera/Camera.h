#pragma once

using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();

	XMMATRIX getViewXM() const { return m_view; }
	XMMATRIX getProjXM() const { return m_proj; }

	void setEye(XMFLOAT3 eye);
	void setOffset(XMFLOAT3 offset);
	void setLookat(XMFLOAT3 lookat);

	void AttachTarget(Transform* target);

	void computeViewMat();
	void computeProjMat();

private:
	// 카메라의 월드 좌표
	XMFLOAT3 m_eye		= XMFLOAT3(0.0f, 0.0f, 0.0f);
	// 카메라 eye 에서 얼마나 떨어져 고정시킬지
	XMFLOAT3 m_offset	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	// 카메라가 바라보는 지점 ( 타겟 )
	XMFLOAT3 m_lookat	= XMFLOAT3(0.0f, 0.0f, -1.0f);
	// 카메라의 윗부분이 어디인지 ( UpVector )
	XMFLOAT3 m_up		= XMFLOAT3(0.0f, 1.0f, 0.0f);

	XMMATRIX m_view		= XMMatrixIdentity();
	XMMATRIX m_proj		= XMMatrixIdentity();
};

