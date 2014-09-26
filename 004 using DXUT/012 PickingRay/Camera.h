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

	// ��ü�� ������ų Ÿ��
	void attachTarget(Transform* target);

	// Mouse Picking
	void Pick(_Out_ XMFLOAT3& rayPos, _Out_ XMFLOAT3& rayDir);

private:
	// ī�޶��� ���� ��ǥ
	XMFLOAT3 m_eye		= XMFLOAT3(0.0f, 0.0f, 0.0f);
	// ī�޶� eye ���� �󸶳� ������ ������ų��
	XMFLOAT3 m_offset	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	// ī�޶� �ٶ󺸴� ���� ( Ÿ�� )
	XMFLOAT3 m_lookat	= XMFLOAT3(0.0f, 0.0f, 1.0f);
	// ī�޶��� ���κ��� ������� ( UpVector )
	XMFLOAT3 m_up		= XMFLOAT3(0.0f, 1.0f, 0.0f);

	XMFLOAT4X4 m_view;
	XMFLOAT4X4 m_proj;
};

