#include "graphics.h"

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

void graphics::ReleaseGraphic(graphics** ppgraphic)
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
bool Initialize()
{
	return true;
}

bool Frame()
{
	if (Render() == false)
		return false; 

	return true;
}

void Release()
{

}

bool Render()
{
	return true;
}

SGAP_END