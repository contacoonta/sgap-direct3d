#include "graphicD3d.h"


SGAP_BEGIN

/*
*/
bool graphicD3d::CreateGraphicD3d(graphicD3d** ppgd3d, int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	if (*ppgd3d != nullptr)
		return false;

	*ppgd3d = new graphicD3d;
	return (*ppgd3d)->Initialize(screenWidth, screenHeight, vsync, hwnd, fullscreen, screenDepth, screenNear);
}

void graphicD3d::DestroyGraphicD3d(graphicD3d** ppgd3d)
{
	if (*ppgd3d)
	{
		(*ppgd3d)->Release();

		delete *ppgd3d;
		*ppgd3d = nullptr;
	}
}

/*
*/
graphicD3d::graphicD3d()
{
}

graphicD3d::graphicD3d(const graphicD3d& d3d)
{
}

graphicD3d::~graphicD3d()
{
}

/*
*/
bool graphicD3d::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	return true;
}

void graphicD3d::Release()
{

}

void graphicD3d::BegineScene()
{

}

void graphicD3d::EndScene()
{

}


SGAP_END