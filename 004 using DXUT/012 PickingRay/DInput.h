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

	//키보드 상태값
	UCHAR					m_keyboardState[256];	
	
	// 마우스 상태값
	DIMOUSESTATE			m_mouseState;
	// 마우스 상대좌표
	INT						m_mouseDelta[2];	
};

#define INPUT DInput::instance()