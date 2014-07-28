#include "application.h"

SGAP_BEGIN

application* g_app = nullptr;

application::application()
{
}

application::application(const application&)
{	
}

application::~application()
{
}

void application::Initialize(LPCWSTR appname, LRESULT (CALLBACK *pWndProc)(HWND, UINT, WPARAM, LPARAM))
{
	DEVMODE dmScreenSettings;
	int posX, posY;

	//자식의 싱글톤 포인터를 부모세대 포인터에 전달.
	g_app = this;

	// Get the instance of this application.
	m_hInst		= GetModuleHandle(NULL);
	m_nameApp	= appname;

	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = pWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, IDI_WINLOGO);
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = m_nameApp;
	
	RegisterClassEx(&wcex);


	// 전체화면 모드
	if (FULLSCREEN)
	{
		// 사용자 시스템 스크린 해상도 크기로 설정
		m_screenWidth = GetSystemMetrics(SM_CXSCREEN);
		m_screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// 전체화면이면 최대화면 크기로 설정 하고 32비트 픽셀로 설정
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)m_screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)m_screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// 전체화면으로 디스플레이 세팅
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// 창 위치는 TOP LEFT
		posX = posY = 0;
	}
	else
	{
		// 창 모드 일때,
		// 윈도우를 화면 중앙에 설정
		m_screenWidth = 800;
		m_screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - m_screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - m_screenHeight) / 2;
	}
	// 윈도우 Ex 생성
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_nameApp, m_nameApp, 
							WS_OVERLAPPEDWINDOW,
							//WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
							posX, posY, m_screenWidth, m_screenHeight, NULL, NULL, m_hInst, NULL);
	
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	// 각종 컴포넌트들 생성 및 초기화
	KEYINPUT::CreateInput(&m_input);
	GRAPHICS::CreateGraphic(&m_graphics);

	// 상속 클래스의 초기화
	OnInit();
}

void application::Run()
{
	MSG msg = { 0 };
	while ( WM_QUIT != msg.message )
	{
		// 윈도 메시지 있다면 , 먼저 처리
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 윈도 메시지가 없다면 , 루프
		else
		{
			if (Mainframe() == false)
				return;
		}

	}//while()
}

bool application::Mainframe()
{
	if (m_input->GetKeyDown(VK_ESCAPE))
		return false;

	if (m_graphics->Frame() == false)
		return false;

	return OnUpdate();
}

void application::Release()
{
	// 각종 컴포넌트들 제거
	GRAPHICS::DestroyGraphic(&m_graphics);
	KEYINPUT::DestroyInput(&m_input);

	// 전체화면 모드 기능 
	if (FULLSCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

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
	case WM_KEYDOWN:
		m_input->KeyDown((UINT)wParam);
		return 0;

	case WM_KEYUP:
		m_input->KeyUp((UINT)wParam);
		return 0;
	}
}

SGAP_END