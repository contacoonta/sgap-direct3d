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
	// ī�޶��� ���� ��ǥ
	XMFLOAT3 m_eye		= XMFLOAT3(0.0f, 0.0f, 0.0f);
	// ī�޶� eye ���� �󸶳� ������ ������ų��
	XMFLOAT3 m_offset	= XMFLOAT3(0.0f, 0.0f, 0.0f);
	// ī�޶� �ٶ󺸴� ���� ( Ÿ�� )
	XMFLOAT3 m_lookat	= XMFLOAT3(0.0f, 0.0f, -1.0f);
	// ī�޶��� ���κ��� ������� ( UpVector )
	XMFLOAT3 m_up		= XMFLOAT3(0.0f, 1.0f, 0.0f);

	XMMATRIX m_view		= XMMatrixIdentity();
	XMMATRIX m_proj		= XMMatrixIdentity();
};

