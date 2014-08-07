#ifndef _GAMEAPP_H_
#define _GMAEAPP_H_

#include "Base/application.h"

using sgap::application;

class GameApp : public application
{
SINGLETON(GameApp)

private:
	GameApp();
	GameApp(const GameApp&);
	~GameApp();
public:
	LRESULT CALLBACK MsgHandler(HWND, UINT, WPARAM, LPARAM);

private:
	void OnInit();
	bool OnUpdate();
	void OnDestroy();
};

#define GAMEAPP GameApp::instance()

#endif