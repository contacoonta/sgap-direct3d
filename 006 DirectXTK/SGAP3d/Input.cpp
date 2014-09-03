#include "stdafx.h"
#include "DXUT.h"
#include "Input.h"

#pragma comment(lib, "dinput8.lib")



Input::Input()
{
	ZeroMemory(m_keyboardState, sizeof(UCHAR)* 256);
	ZeroMemory(&m_mouseState, sizeof(DIMOUSESTATE));
	ZeroMemory(m_mouseDelta, sizeof(INT)* 2);
	
	Initialize();
}

Input::~Input()
{
	Release();
}


/*
*/
BOOL Input::isKeyPressed(WORD key)
{
	if (m_keyboardState[key] & 0x80)
		return true;

	return false;
}

/*
	���콺 ���� ��ǥ
*/
POINT Input::getMousePos()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(DXUTGetHWND(), &pt);

	return pt;
}

/*
*/
void Input::getMousePosWorld(_Out_ XMVECTOR& worldPos, _Out_ XMVECTOR& worldDir, _In_ XMMATRIX view, _In_ XMMATRIX proj)
{
	const DXGI_SURFACE_DESC* pd3dsdBackBuffer = DXUTGetDXGIBackBufferSurfaceDesc();

	POINT		ptcursor	= getMousePos();
	XMFLOAT4X4	mproj		= XMFLOAT4X4();
	XMFLOAT3	viewpos		= XMFLOAT3();

	XMStoreFloat4x4(&mproj, proj);

	// ���콺 ��ġ ȭ����ǥ(Screen Space) -> ����ǥ(View Space) �� ��ȯ
	viewpos.x = (((ptcursor.x * 2.0f) / pd3dsdBackBuffer->Width) - 1) / mproj._11;
	viewpos.y = -(((ptcursor.x * 2.0f) / pd3dsdBackBuffer->Width) - 1) / mproj._22;
	viewpos.z = 1.0f;
	
	XMVECTOR viewspacePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR viewspaceDir = XMVectorSet(viewpos.x, viewpos.y, viewpos.z, 0.0f);
	// ȭ�� �߾� ���� ( FPS ������ ���� )
	//XMVECTOR rayPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	XMVECTOR tmpDeter;
	// ViewSpace matrix �� ����ķ� WorldSpace matrix ���Ѵ�.
	XMMATRIX mworld = XMMatrixInverse(&tmpDeter, view);

	worldPos = XMVector3TransformCoord(viewspacePos, mworld);
	worldDir = XMVector3TransformCoord(viewspaceDir, mworld);
}

/*
	���콺 ��ȭ��
*/
INT* Input::getMouseDelta()
{
	return m_mouseDelta;
}

/*
*/
HRESULT Input::Initialize()
{
	HRESULT hr = S_OK;
	
	Release();
	//grabMouse();
	
	m_mouseDelta[0] = 0;	// MOUSE DELTA X
	m_mouseDelta[1] = 0;	// MOUSE DELTA Y

	// DirectInput ��ġ ����
	hr = DirectInput8Create(DXUTGetHINSTANCE(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_dinput, NULL);
	if (FAILED(hr))
		return hr;

	// Ű���� ���� �� ����
	hr = m_dinput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	hr = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = m_keyboard->SetCooperativeLevel(DXUTGetHWND(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	hr = m_keyboard->Acquire();


	// ���콺 ���� �� ����
	hr = m_dinput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	hr = m_mouse->SetDataFormat(&c_dfDIMouse/*&g_dfMouse*/);
	// NON-EXCULUSIVE ���¶��, 
	// ��Ŀ���� �Ҿ��� ����� �׻� üũ�ؾ��Ѵ�.
	hr = m_mouse->SetCooperativeLevel(DXUTGetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	hr = m_mouse->Acquire();

	//�����ǥ, ������ǥ ����
	/*DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = DIPROPAXISMODE_ABS;
	m_mouse->SetProperty(DIPROP_AXISMODE, &dipdw.diph);*/

	return hr;
}

HRESULT Input::Update()
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

void Input::Release()
{
	if (m_mouse)	m_mouse->Unacquire();
	if (m_keyboard)	m_keyboard->Unacquire();

	SAFE_RELEASE(m_keyboard);
	SAFE_RELEASE(m_mouse);
	SAFE_RELEASE(m_dinput);
}