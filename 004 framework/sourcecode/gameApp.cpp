#include "gameApp.h"

GameApp::GameApp()
{
}

GameApp::~GameApp()
{
}

/*
*/
void GameApp::OnInit()
{
}

bool GameApp::OnUpdate()
{
	return true;
}

void GameApp::OnDestroy()
{
}

/*
*/
LRESULT CALLBACK GameApp::MsgHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	application::MsgHandler(hWnd, message, wParam, lParam);

	switch (message)
	{
	case WM_NULL:
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}