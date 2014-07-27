#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#include "basedef.h"
#include "graphicD3d.h"

SGAP_BEGIN

//TEMP VALUES
const BOOL	FULLSCREEN	= FALSE;
const BOOL	VERTICALSYNC = FALSE;
const FLOAT	SCREENDEPTH = 1000.0f;
const FLOAT	SCREENNEAR = 0.1f;


typedef class graphics
{
public:
	static bool CreateGraphic(graphics** ppgraphic);
	static void DestroyGraphic(graphics** ppgraphic);

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
	LPGRAPHICD3D	m_d3d = nullptr;
	
}GRAPHICS, *LPGRAPHICS;

SGAP_END

#endif