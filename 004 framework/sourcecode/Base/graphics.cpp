#include "graphics.h"
#include "application.h"
#include "../gameApp.h"

SGAP_BEGIN

/*
*/
bool graphics::CreateGraphic(graphics** ppgraphic)
{
	if (*ppgraphic != nullptr)
		return false;

	*ppgraphic = new graphics;

	return (*ppgraphic)->Initialize();
}

void graphics::DestroyGraphic(graphics** ppgraphic)
{
	if (*ppgraphic)
	{
		(*ppgraphic)->Release();

		delete *ppgraphic;
		*ppgraphic = nullptr;
	}
}


/*
*/
graphics::graphics()
{
}

graphics::graphics(const graphics&)
{
}

graphics::~graphics()
{
}


/*
*/
bool graphics::Initialize()
{
	if (graphicD3d::CreateGraphicD3d(&m_d3d, g_app->Width(), g_app->Height(), VERTICALSYNC, g_app->Hwnd(), FULLSCREEN, SCREENDEPTH, SCREENNEAR) == FALSE)
	{
		MessageBox(g_app->Hwnd(), L"Direct3D initialize FAILED", L"ERROR", MB_OK);
		return false;
	}
	
	return true;
}

bool graphics::Frame()
{
	if (Render() == false)
		return false; 

	return true;
}

void graphics::Release()
{
	graphicD3d::DestroyGraphicD3d(&m_d3d);
}

bool graphics::Render()
{
	if (m_d3d == nullptr)
		return FALSE;

	m_d3d->BegineScene();
	
	//draw to anything...

	m_d3d->EndScene();

	return true;
}

SGAP_END