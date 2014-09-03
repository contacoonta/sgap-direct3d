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
	마우스 절대 좌표
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

	// 마우스 위치 화면좌표(Screen Space) -> 뷰좌표(View Space) 로 변환
	viewpos.x = (((ptcursor.x * 2.0f) / pd3dsdBackBuffer->Width) - 1) / mproj._11;
	viewpos.y = -(((ptcursor.x * 2.0f) / pd3dsdBackBuffer->Width) - 1) / mproj._22;
	viewpos.z = 1.0f;
	
	XMVECTOR viewspacePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR viewspaceDir = XMVectorSet(viewpos.x, viewpos.y, viewpos.z, 0.0f);
	// 화면 중앙 고정 ( FPS 게임을 위한 )
	//XMVECTOR rayPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	XMVECTOR tmpDeter;
	// ViewSpace matrix 의 역행렬로 WorldSpace matrix 구한다.
	XMMATRIX mworld = XMMatrixInverse(&tmpDeter, view);

	worldPos = XMVector3TransformCoord(viewspacePos, mworld);
	worldDir = XMVector3TransformCoord(viewspaceDir, mworld);
}

/*
	마우스 변화량
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

	// DirectInput 장치 생성
	hr = DirectInput8Create(DXUTGetHINSTANCE(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_dinput, NULL);
	if (FAILED(hr))
		return hr;

	// 키보드 생성 및 설정
	hr = m_dinput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	hr = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = m_keyboard->SetCooperativeLevel(DXUTGetHWND(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	hr = m_keyboard->Acquire();


	// 마우스 생성 및 설정
	hr = m_dinput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	hr = m_mouse->SetDataFormat(&c_dfDIMouse/*&g_dfMouse*/);
	// NON-EXCULUSIVE 상태라면, 
	// 포커스를 잃었나 얻었나 항상 체크해야한다.
	hr = m_mouse->SetCooperativeLevel(DXUTGetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	hr = m_mouse->Acquire();

	//상대좌표, 절대좌표 설정
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

	//키보드 상태 조회
	//hr = m_keyboard->Poll();
	hr = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(hr))
	{
		//윈도 최소화등 디바이스 손실로, 키보드 포커스를 잃으면 acquire 실패
		//다시 복쉬하면 acquire 시도
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) {
			m_keyboard->Acquire();
		}
		else {
			return hr;
		}
	}
	
	//마우스 상태 조회
	hr = m_mouse->Poll();
	if (FAILED(hr))
	{
		hr = m_mouse->Acquire();
		while (hr == DIERR_INPUTLOST)
			hr = m_mouse->Acquire();

		return S_OK;
	}

	//마우스는 절대 위치가 아닌 , 생성시점 기준의 상대 위치(Delta position)로 저장. 
	hr = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(hr))
		return hr;
	
		
	//마우스 위치 적용
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