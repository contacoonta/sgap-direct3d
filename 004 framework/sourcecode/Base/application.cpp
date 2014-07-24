#include "application.h"

application::application()
{
}

application::~application()
{
}

void application::Initialize(LPCWSTR appname, LRESULT (CALLBACK *pWndProc)(HWND, UINT, WPARAM, LPARAM))
{
	DEVMODE dmScreenSettings;
	int screenWidth, screenHeight;
	int posX, posY;

	// Get the instance of this application.
	m_hInst		= GetModuleHandle(NULL);
	m_nameApp	= appname;

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


	// ��üȭ�� ���
	if (FULLSCREEN)
	{
		// ����� �ý��� ��ũ�� �ػ� ũ��� ����
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// ��üȭ���̸� �ִ�ȭ�� ũ��� ���� �ϰ� 32��Ʈ �ȼ��� ����
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// ��üȭ������ ���÷��� ����
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// â ��ġ�� TOP LEFT
		posX = posY = 0;
	}
	else
	{
		// â ��� �϶�,
		// �����츦 ȭ�� �߾ӿ� ����
		screenWidth = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}
	// ������ Ex ����
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_nameApp, m_nameApp, 
							WS_OVERLAPPEDWINDOW,
							//WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
							posX, posY, screenWidth, screenHeight, NULL, NULL, m_hInst, NULL);

	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	// input ���� �� �ʱ�ȭ.
	INPUT::CreateInput(&m_input);

	// ��� Ŭ������ �ʱ�ȭ
	OnInit();
}

void application::Run()
{
	MSG msg = { 0 };
	while ( WM_QUIT != msg.message )
	{
		// ���� �޽��� �ִٸ� , ���� ó��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// ���� �޽����� ���ٸ� , ����
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
	{
		return false;
	}

	return OnUpdate();
}

void application::Release()
{
	// ��ǲ ����
	INPUT::ReleaseInput(&m_input);

	// ��üȭ�� ��� ��� 
	if (FULLSCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

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
	case WM_KEYDOWN:
		m_input->KeyDown((UINT)wParam);
		return 0;

	case WM_KEYUP:
		m_input->KeyUp((UINT)wParam);
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
