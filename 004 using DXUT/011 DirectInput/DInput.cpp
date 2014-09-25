#include "DXUT.h"
#include "DInput.h"

#pragma comment(lib, "dinput8.lib")


DInput::DInput()
{
	Initialize();
}

DInput::DInput(const DInput&)
{
}

DInput::~DInput()
{
	Release();
}


/*
*/
HRESULT DInput::Initialize()
{
	HRESULT hr = S_OK;
	Release();

	m_mouseDelta[0] = 0;	// MOUSE DELTA X
	m_mouseDelta[1] = 0;	// MOUSE DELTA Y

	// DirectInput ��ġ ����
	hr = DirectInput8Create(DXUTGetHINSTANCE(), 
						DIRECTINPUT_VERSION, IID_IDirectInput8, 
						(void**)&m_dinput, NULL);
	if (FAILED(hr))
		return hr;

	// Ű���� ���� �� ����
	hr = m_dinput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	hr = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = m_keyboard->SetCooperativeLevel(DXUTGetHWND(), 
									DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	hr = m_keyboard->Acquire();


	// ���콺 ���� �� ����
	hr = m_dinput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	hr = m_mouse->SetDataFormat(&c_dfDIMouse);
	hr = m_mouse->SetCooperativeLevel(DXUTGetHWND(), 
									DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	hr = m_mouse->Acquire();

	return hr;
}

HRESULT DInput::Update()
{
	HRESULT hr = S_OK;

	//Ű���� ���� ��ȸ
	//hr = m_keyboard->Poll();
	hr = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(hr))
	{
		//���� �ּ�ȭ�� ����̽� �սǷ�, Ű���� ��Ŀ���� ������ acquire ����
		//�ٽ� �����ϸ� acquire �õ�
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) {
			m_keyboard->Acquire();
		}
		else {
			return hr;
		}
	}
	
	//���콺 ���� ��ȸ
	hr = m_mouse->Poll();
	if (FAILED(hr))
	{
		hr = m_mouse->Acquire();
		while (hr == DIERR_INPUTLOST)
			hr = m_mouse->Acquire();

		return S_OK;
	}

	//���콺�� ���� ��ġ�� �ƴ� , �������� ������ ��� ��ġ(Delta position)�� ����. 
	hr = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(hr))
		return hr;	
		
	//���콺 ��ġ ����
	m_mouseDelta[0] += m_mouseState.lX /*- grabPos_.x*/;
	m_mouseDelta[1] += m_mouseState.lY /*- grabPos_.y*/;

	return hr;
}

void DInput::Release()
{
	if (m_mouse)	m_mouse->Unacquire();
	if (m_keyboard)	m_keyboard->Unacquire();

	SAFE_RELEASE(m_keyboard);
	SAFE_RELEASE(m_mouse);
	SAFE_RELEASE(m_dinput);
}


BOOL DInput::isKeyPressed(WORD key)
{
	if (m_keyboardState[key] & 0x80)
		return true;

	return false;
}


/*
���콺 ��ȭ��
*/
INT* DInput::getMouseDelta()
{
	return m_mouseDelta;
}