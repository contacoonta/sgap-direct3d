#pragma once

#include "Base/application.h"

class GameApp : public application
{
SINGLETON(GameApp)

public:
	GameApp();
	~GameApp();

	LRESULT CALLBACK MsgHandler(HWND, UINT, WPARAM, LPARAM);

private:
	void OnInit();
	bool OnUpdate();
	void OnDestroy();
};

#define GAMEAPP GameApp::instance()