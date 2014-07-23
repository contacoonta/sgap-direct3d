#pragma once

#include <Windows.h>

#define WIN32_LEAN_AND_MEAN

#define SINGLETON(x) public : static x& instance() { static x inst; return inst; }

// global values
const bool FULLSCREEN = false;

class application
{
public:
			application();
	virtual ~application();

	HINSTANCE	hinst() const	{ return m_hInst;	}
	HWND		hwnd() const	{ return m_hWnd;	}
	LPCWSTR		appname() const	{ return m_nameApp; }

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
	HINSTANCE	m_hInst = NULL;
	HWND		m_hWnd = NULL;
	LPCWSTR		m_nameApp = L"";
};

