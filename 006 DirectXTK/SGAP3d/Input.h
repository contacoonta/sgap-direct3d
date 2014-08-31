#pragma once

#include <dinput.h>

#define DIRECTINPUT_VERSION 0X0800


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
	//void					grabMouse()		{ bgrab_ = true; }

	BOOL					isLBtnDown()	{ return m_mouseState.rgbButtons[0] & 0x80; }
	BOOL					isMBtnDown()	{ return m_mouseState.rgbButtons[2] & 0x80; }
	BOOL					isRBtnDown()	{ return m_mouseState.rgbButtons[1] & 0x80; }

private:
	IDirectInput8*			m_dinput	= nullptr;
	IDirectInputDevice8*	m_keyboard	= nullptr;
	IDirectInputDevice8*	m_mouse		= nullptr;
	
	UCHAR					m_keyboardState[256];
	
	DIMOUSESTATE			m_mouseState;
	INT						m_mouseDelta[2];
	
private:
	/*POINT					grabPos_;
	BOOL					bgrab_		= false;*/

};
