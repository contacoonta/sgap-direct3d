#include "application.h"

application::application()
{
}

application::~application()
{
}

void application::Initialize(LPCWSTR appname, LRESULT (CALLBACK *pWndProc)(HWND, UINT, WPARAM, LPARAM))
{
	int screenWidth, screenHeight;
	int posX, posY;

	// Get the instance of this application.
	m_hInst = GetModuleHandle(NULL);

	m_nameApp = appname;

	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = pWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = m_nameApp;
	wcex.hIconSm = LoadIcon(wcex.hInstance, NULL);
	
	RegisterClassEx(&wcex);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	

	// 전체화면 모드 추가해야...


	// 창 모드 일때,
	// 윈도우를 화면 중앙에 설정
	screenWidth = 800;
	screenHeight = 600;

	posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	// 윈도우 Ex 생성
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_nameApp, m_nameApp, 
							WS_OVERLAPPEDWINDOW,
							posX, posY, screenWidth, screenHeight, NULL, NULL, m_hInst, NULL);

	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	//ShowCursor(false);
}

void application::Run()
{
	// Main message loop
	MSG msg = { 0 };
	while ( WM_QUIT != msg.message )
	{
		// 윈도 메시지 먼저 처리
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 메시지가 없다면 기본 app loop
		else
		{
			
		}

	}//while()
}

void application::Release()
{

	//ShowCursor(true);

	// 전체화면 모드 기능 

	// 윈도 제거
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;

	// app instance 제거
	UnregisterClass(m_nameApp, m_hInst);
	m_hInst = NULL;
	
}

LRESULT CALLBACK application::MsgHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}


