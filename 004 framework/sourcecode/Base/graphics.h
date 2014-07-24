#pragma once

#include "basedef.h"

SGAP_BEGIN

//TEMP VALUES
const bool FULLSCREEN = false;


typedef class graphics
{
public:
	static bool CreateGraphic(graphics** ppgraphic);
	static void ReleaseGraphic(graphics** ppgraphic);

public:
	graphics();
	graphics(const graphics&);
	~graphics();

	bool Initialize();
	bool Frame();
	void Release();

private:
	bool Render();

private:
	
}GRAPHICS;

SGAP_END