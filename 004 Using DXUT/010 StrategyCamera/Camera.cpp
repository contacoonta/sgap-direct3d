
#include "DXUT.h"
#include "Camera.h"


camera::camera()
{
	XMStoreFloat4x4(&m_view, XMMatrixIdentity());
	XMStoreFloat4x4(&m_proj, XMMatrixIdentity());
}

camera::~camera()
{
}


/*
*/
XMMATRIX camera::getViewXM() const
{
	return XMLoadFloat4x4(&m_view);
}

XMFLOAT4X4 camera::getView() const
{
	return m_view;
}


/*
*/
XMMATRIX camera::getProjXM() const
{
	return XMLoadFloat4x4(&m_proj);
}

XMFLOAT4X4 camera::getProj() const
{
	return m_proj;
}


/*
*/
void camera::Update(float felapsed)
{
	//m_view = XMMatrixLookAtLH(Eye, LookAt, Up);
}