#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#define APP application::getinstance()

class application
{
public:
	static application& getinstance()
	{
		static application instance;
		return instance;
	}

public:
	application();
	~application();

	void Initialize(LPCWSTR appname, LRESULT(CALLBACK *pWndProc)(HWND, UINT, WPARAM, LPARAM));
	void Run();
	void Release();

	LRESULT CALLBACK MsgHandler(HWND, UINT, WPARAM, LPARAM);

private:
	HINSTANCE               m_hInst = NULL;
	HWND                    m_hWnd = NULL;
	LPCWSTR					m_nameApp = L"";
};