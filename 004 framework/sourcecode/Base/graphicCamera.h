#ifndef _GRAPHICCAMERA_H_
#define _GRAPHICCAMERA_H_

#include "basedef.h"


SGAP_BEGIN

typedef class iCamera
{
public:
					iCamera()				{}	
					iCamera(const iCamera&) {}
	virtual			~iCamera()				{}

public:	
	XMFLOAT3		GetRotation()	{ return m_rotation; }
	XMFLOAT3		GetPosition()	{ return m_position; }
	
	XMMATRIX		GetMatrixView()	{ return m_viewMatrix; }
	
	void			SetPosition(XMFLOAT3 pos)	{ m_position = pos; }
	void			SetRotation(XMFLOAT3 rot)	{ m_rotation = rot; }

	virtual void	initialize() = 0;
	virtual void	release() = 0;
	virtual void	render();

protected:
	XMFLOAT3		m_position;
	XMFLOAT3		m_rotation;
	XMMATRIX		m_viewMatrix;

}iCAMERA, *LPCAMERA;



/*
*/
typedef class graphicCamera : public iCamera
{
public:
	static bool Create(iCamera** ppcamera);
	static void Destroy(iCamera** ppcamera);

public:
	graphicCamera();
	graphicCamera(const graphicCamera&);
	~graphicCamera();

	void initialize();
	void release();

}GRAPHICCAMERA;



SGAP_END

#endif