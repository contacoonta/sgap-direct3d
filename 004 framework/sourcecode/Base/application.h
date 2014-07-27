
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "basedef.h"
#include "input.h"
#include "graphics.h"

SGAP_BEGIN

class application
{
protected:
			application();
			application(const application&);
	virtual ~application();

public:
	HINSTANCE	Hinst() const	{ return m_hInst;	}
	HWND		Hwnd() const	{ return m_hWnd;	}
	LPCWSTR		Appname() const	{ return m_nameApp; }
	INT			Width() const	{ return m_screenWidth; }
	INT			Height() const	{ return m_screenHeight; }

	iINPUT*		Input() const		{ return m_input; }
	GRAPHICS*	Graphics() const	{ return m_graphics; }
	

	void Initialize(application* papp, LPCWSTR appname, LRESULT(CALLBACK *pWndProc)(HWND, UINT, WPARAM, LPARAM));
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

	INT			m_screenWidth	= 0;
	INT			m_screenHeight	= 0;

	LPINPUT		m_input			= nullptr;
	LPGRAPHICS	m_graphics		= nullptr;
};

// 자식 클래스의 싱글톤 객체 포인터를 담기 위한 부모세대의 포인터, cpp에서 초기화 한다.
extern application* g_app;

SGAP_END


#endif