#pragma once

#include "base.h"
#include <dinput.h>

#define DIRECTINPUT_VERSION 0x0800

class DInput
{
SINGLETON(DInput);

public:
	HRESULT		Initialize();
	HRESULT		Update();
	void		Release();
	
	// Keyboard
	BOOL		isKeyPressed(WORD key);

	// Mouse 
	INT*		getMouseDelta();

	BOOL		isLBtnDown();
	BOOL		isMBtnDown();
	BOOL		isRBtnDown();

private:
	IDirectInput8*			m_dinput = nullptr;
	IDirectInputDevice8*	m_keyboard = nullptr;
	IDirectInputDevice8*	m_mouse = nullptr;

	//Ű���� ���°�
	UCHAR					m_keyboardState[256];	
	
	// ���콺 ���°�
	DIMOUSESTATE			m_mouseState;
	// ���콺 �����ǥ
	INT						m_mouseDelta[2];	
};

#define INPUT DInput::instance()