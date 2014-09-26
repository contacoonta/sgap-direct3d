#pragma once

using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();

	XMMATRIX getViewXM() const;
	XMMATRIX getProjXM() const;

	void setEye(XMFLOAT3 eye);
	void setOffset(XMFLOAT3 offset);
	void setLookat(XMFLOAT3 lookat);

	void computeViewMat();
	void computeProjMat();

	// 물체에 부착시킬 타겟
	void attachTarget(Transform* target);

	// Mouse Picking
	void Pick(_Out_ XMFLOAT3& rayPos, _Out_ XMFLOAT3& rayDir);

private:
	// 카메라의 월드 좌표
	XMFLOAT3 m_eye		= XMFLOAT3(0.0f, 0.0f, 0.0f);
	// 카메라 eye 에서 얼마나 떨어져 고정시킬지
	XMFLOAT3 m_offset	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	// 카메라가 바라보는 지점 ( 타겟 )
	XMFLOAT3 m_lookat	= XMFLOAT3(0.0f, 0.0f, 1.0f);
	// 카메라의 윗부분이 어디인지 ( UpVector )
	XMFLOAT3 m_up		= XMFLOAT3(0.0f, 1.0f, 0.0f);

	XMFLOAT4X4 m_view;
	XMFLOAT4X4 m_proj;
};

