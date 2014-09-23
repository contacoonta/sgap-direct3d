#include "DXUT.h"
#include "FW1Precompiled.h"
#include "DwriteText.h"

using namespace DirectX;


DwriteText::DwriteText()
{
	Initialize();
}

DwriteText::DwriteText(const DwriteText&)
{
}

DwriteText::~DwriteText()
{
	Release();
}


HRESULT DwriteText::Initialize()
{
	HRESULT hr = S_OK;

	hr = FW1CreateFactory(FW1_VERSION, &m_factory);
	hr = m_factory->CreateFontWrapper(DXUTGetD3D11Device(), L"¸¼Àº°íµñ", &m_fontwrap);

	return hr;
}

void DwriteText::Release()
{
	SAFE_RELEASE(m_factory);
	SAFE_RELEASE(m_fontwrap);
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
							0xff0000ff,// Text color, 0xAaBbGgRr
							FW1_NOGEOMETRYSHADER | FW1_RESTORESTATE // Flags	
							);
	
}

/*
*/
void DwriteText::AddString(std::wstring wstr)
{
	m_strings.push_back(wstr);
}

void DwriteText::RenderStrings()
{
	int posX = 10;
	int posY = 10;

	for ( auto x : m_strings )
	{
		Render(x, 0, posX, posY, 20.0f);
		posY += m_height;
	}
}