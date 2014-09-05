
#include "stdafx.h"
#include "dxut.h"
#include "FW1Precompiled.h"
#include "SDKmisc.h"
#include "Mesh.h"
#include "DwriteText.h"

#include <string>


DwriteText::DwriteText()
{	
	Initialize();
}

DwriteText::~DwriteText()
{
	Release();
}

HRESULT DwriteText::Initialize()
{
	HRESULT hr = S_OK;
	
	hr = FW1CreateFactory(FW1_VERSION, &m_factory);
	hr = m_factory->CreateFontWrapper(DXUTGetD3D11Device(), L"Arial", &m_fontwrap);

	return hr;
}


/*
*/
void DwriteText::Render(std::wstring wtext, INT numex, int x, int y, float fsize )
{
	HRESULT hr = S_OK;

	std::wstring str;
	if (numex == 0)
	{
		str = wtext;
	}
	else
	{
		str = wtext + std::to_wstring(numex);
	}

	m_fontwrap->DrawString(	DXUTGetD3D11DeviceContext(),
							str.c_str(),// String
							fsize,// Font size
							x,// X position
							y,// Y position
							0xff0099ff,// Text color, 0xAaBbGgRr
							FW1_NOGEOMETRYSHADER // Flags	
							);
	
}

void DwriteText::Release()
{
	SAFE_RELEASE(m_factory);
	SAFE_RELEASE(m_fontwrap);
}