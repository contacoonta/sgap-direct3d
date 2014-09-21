#pragma once

#include <dinput.h>
#include <DirectXMath.h>

#define DIRECTINPUT_VERSION 0X0800

using namespace DirectX;


class Input
{
public:
	Input();
	~Input();

public:
	HRESULT					Initialize();
	HRESULT					Update();
	void					Release();

	//KEYBOARD
	BOOL					isKeyPressed(WORD key);

	//MOUSE
	POINT					getMousePos();
	INT*					getMouseDelta();
	
	void					Pick(_Out_ XMVECTOR& rayPos, _Out_ XMVECTOR& rayDir, _In_ XMMATRIX& world, _In_ XMMATRIX& view, _In_ XMMATRIX& proj);


	BOOL					isLBtnDown()	{ return m_mouseState.rgbButtons[0] & 0x80; }
	BOOL					isMBtnDown()	{ return m_mouseState.rgbButtons[2] & 0x80; }
	BOOL					isRBtnDown()	{ return m_mouseState.rgbButtons[1] & 0x80; }

private:
	IDirectInput8*			m_dinput	= nullptr;
	IDirectInputDevice8*	m_keyboard	= nullptr;
	IDirectInputDevice8*	m_mouse		= nullptr;
	
	UCHAR					m_keyboardState[256];	//키보드 상태값
	
	DIMOUSESTATE			m_mouseState;		// 마우스 상태값
	INT						m_mouseDelta[2];	// 마우스 상대좌표

};
