/*
	전략 카메라 	
*/

#pragma once

#include "Transform.h"

class camera : public Transform
{
public:
	camera();
	~camera();

	void			Update(float felapsed);

	XMMATRIX		getViewXM() const;
	XMFLOAT4X4		getView() const;

	XMMATRIX		getProjXM() const;
	XMFLOAT4X4		getProj() const;

private:
	XMFLOAT4X4		m_view;
	XMFLOAT4X4		m_proj;
};