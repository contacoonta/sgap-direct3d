#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#include "basedef.h"
#include "graphicD3d.h"
#include "graphicFont.h"


SGAP_BEGIN

//TEMP VALUES
const BOOL	FULLSCREEN	= FALSE;
const BOOL	VERTICALSYNC = TRUE;
const FLOAT	SCREENDEPTH = 1000.0f;
const FLOAT	SCREENNEAR = 0.1f;


typedef class graphics
{
public:
	static bool Create(graphics** ppgraphic);
	static void Destroy(graphics** ppgraphic);

public:
	graphics();
	graphics(const graphics&);
	~graphics();

	LPGRAPHICD3D	getD3d() { return m_d3d; }

	bool Initialize();
	bool Frame();
	void Release();

private:
	bool Render();

private:
	LPGRAPHICD3D	m_d3d = nullptr;
	LPGRAPHICFONT	m_font = nullptr;
	
}GRAPHICS, *LPGRAPHICS;

SGAP_END

#endif