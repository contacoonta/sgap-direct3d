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
	���콺 ��ȭ��
*/
INT* Input::getMouseDelta()
{
	return m_mouseDelta;
}


void Input::Pick(_Out_ XMVECTOR& rayPos, _Out_ XMVECTOR& rayDir, _In_ XMMATRIX& world, _In_ XMMATRIX& view, _In_ XMMATRIX& proj)
{
	const DXGI_SURFACE_DESC* pd3dsdBackBuffer = DXUTGetDXGIBackBufferSurfaceDesc();

	POINT ptcursor = getMousePos();

	// Compute the vector of the pick ray in screen space
	XMFLOAT4X4	mproj = XMFLOAT4X4();
	XMStoreFloat4x4(&mproj, proj);

	XMFLOAT3 v;
	v.x = (((2.0f * ptcursor.x) / pd3dsdBackBuffer->Width) - 1) / mproj._11;
	v.y = -(((2.0f * ptcursor.y) / pd3dsdBackBuffer->Height) - 1) / mproj._22;
	v.z = 1.0f;

	// Get the inverse view matrix	
	XMMATRIX mWorldView = world * view;
	//XMVECTOR determin;
	XMMATRIX mInv = XMMatrixInverse(NULL, mWorldView);

	XMFLOAT4X4	m = XMFLOAT4X4();
	XMStoreFloat4x4(&m, mInv);

	// Transform the screen space pick ray into 3D space
	XMFLOAT3 pickRayDir;
	pickRayDir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
	pickRayDir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
	pickRayDir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
	rayDir = XMLoadFloat3(&pickRayDir);

	XMFLOAT3 pickRayOrig;
	pickRayOrig.x = m._41;
	pickRayOrig.y = m._42;
	pickRayOrig.z = m._43;
	rayPos = XMLoadFloat3(&pickRayOrig);
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