#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "basedef.h"
#include "input.h"
#include "graphics.h"

SGAP_BEGIN

class application
{
public:
			application();
	virtual ~application();

	HINSTANCE	Hinst() const	{ return m_hInst;	}
	HWND		Hwnd() const	{ return m_hWnd;	}
	LPCWSTR		Appname() const	{ return m_nameApp; }

	iINPUT*		Input() const		{ return m_input; }
	GRAPHICS*	Graphics() const	{ return m_graphics; }
	

	void Initialize(LPCWSTR appname, LRESULT(CALLBACK *pWndProc)(HWND, UINT, WPARAM, LPARAM));
	void Run();
	void Release();	
	bool Mainframe();	// app 의 주요 업데이트가 일어나는 곳

	virtual LRESULT CALLBACK MsgHandler(HWND, UINT, WPARAM, LPARAM);

protected:
	virtual void OnInit()		= 0;
	virtual bool OnUpdate()		= 0;
	virtual void OnDestroy()	= 0;

private:
	HINSTANCE	m_hInst			= NULL;
	HWND		m_hWnd			= NULL;
	LPCWSTR		m_nameApp		= L"";

	iINPUT*		m_input			= nullptr;
	GRAPHICS*	m_graphics		= nullptr;
};

SGAP_END