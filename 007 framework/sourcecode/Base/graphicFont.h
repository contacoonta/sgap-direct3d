#ifndef _GRAPHICFONT_H_
#define _GRAPHICFONT_H_

#pragma comment(lib, "libraries/x86/FW1FontWrapper.lib")

#include "basedef.h"
#include "../libraries/FW1FontWrapper.h"

#include "graphicD3d.h"


SGAP_BEGIN

typedef class graphicFont
{
public:
	static bool Create(graphicFont** ppfont, LPGRAPHICD3D pd3d);
	static void Destroy(graphicFont** ppfont);

public:
	graphicFont();
	graphicFont(const graphicFont&);
	~graphicFont();

	bool Initialize(LPGRAPHICD3D pd3d);
	void Release();

	void DrawString(LPCWSTR str, float posX, float posY);

private:
	IFW1Factory*		m_factory	= nullptr;
	IFW1FontWrapper*	m_fontwrap	= nullptr;

	LPGRAPHICD3D		pd3d_ = nullptr;

}GRAPHICFONT, *LPGRAPHICFONT;


SGAP_END

#endif