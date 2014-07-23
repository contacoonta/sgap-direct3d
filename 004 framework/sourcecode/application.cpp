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
	

	// ��üȭ�� ��� �߰��ؾ�...


	// â ��� �϶�,
	// �����츦 ȭ�� �߾ӿ� ����
	screenWidth = 800;
	screenHeight = 600;

	posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	// ������ Ex ����
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
		// ���� �޽��� ���� ó��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// �޽����� ���ٸ� �⺻ app loop
		else
		{
			
		}

	}//while()
}

void application::Release()
{

	//ShowCursor(true);

	// ��üȭ�� ��� ��� 

	// ���� ����
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;

	// app instance ����
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


