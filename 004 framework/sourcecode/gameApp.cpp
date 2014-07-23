#include "gameApp.h"

GameApp::GameApp()
{
}

GameApp::~GameApp()
{
}

LRESULT CALLBACK GameApp::MsgHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	application::MsgHandler(hWnd, message, wParam, lParam);

	switch (message)
	{
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

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