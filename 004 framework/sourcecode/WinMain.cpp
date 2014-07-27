#include "gameApp.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	UNREFERENCED_PARAMETER( hInstance );
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );
	UNREFERENCED_PARAMETER( nCmdShow );

	GAMEAPP.Initialize(&GAMEAPP, L"GameApp", WndProc);
	GAMEAPP.Run();
	GAMEAPP.Release();

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		return 0;

	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

		// 기타 다른 메시지는 app 의 메시지 핸들러에 전달
	default:
		return GAMEAPP.MsgHandler(hWnd, message, wParam, lParam);
	}
}