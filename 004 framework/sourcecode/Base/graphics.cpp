#include "graphics.h"
#include "application.h"
#include "../gameApp.h"

SGAP_BEGIN

/*
*/
bool graphics::Create(graphics** ppgraphic)
{
	if (*ppgraphic != nullptr)
		return false;

	*ppgraphic = new graphics;

	return (*ppgraphic)->Initialize();
}

void graphics::Destroy(graphics** ppgraphic)
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
	if (graphicD3d::Create(&m_d3d, g_app->Width(), g_app->Height(), VERTICALSYNC, g_app->Hwnd(), FULLSCREEN, SCREENDEPTH, SCREENNEAR) == FALSE)
	{
		MessageBox(g_app->Hwnd(), L"Direct3D initialize FAILED", L"ERROR", MB_OK);
		return false;
	}

	// DirectWrite 폰트 생성
	if (graphicFont::Create(&m_font, m_d3d) == FALSE)
	{
		MessageBox(g_app->Hwnd(), L"Direct Write Font create FAILED", L"ERROR", MB_OK);
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
	graphicFont::Destroy(&m_font);
	graphicD3d::Destroy(&m_d3d);
}

bool graphics::Render()
{
	if (m_d3d == nullptr)
		return FALSE;

	m_d3d->BegineScene();
	{	
		m_font->DrawString(L"Direct Write with D3d11", 10.0f, 100.0f);
		m_font->DrawString(L"다이렉트 라이트 디3디11", 10.0f, 200.0f);		
	}
	m_d3d->EndScene();

	return true;
}

SGAP_END