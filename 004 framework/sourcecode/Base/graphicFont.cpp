#include "graphicFont.h"
#include "graphicD3d.h"

SGAP_BEGIN

/*
*/
bool graphicFont::Create(graphicFont** ppfont, LPGRAPHICD3D pd3d)
{
	if (*ppfont != nullptr)
		return false;

	*ppfont = new graphicFont;
	return (*ppfont)->Initialize(pd3d);
}

void graphicFont::Destroy(graphicFont** ppfont)
{
	if (*ppfont)
	{
		(*ppfont)->Release();

		delete *ppfont;
		*ppfont = nullptr;
	}
}

/*
*/
graphicFont::graphicFont()
{

}

graphicFont::graphicFont(const graphicFont&)
{

}

graphicFont::~graphicFont()
{

}


/*
*/
bool graphicFont::Initialize(LPGRAPHICD3D pd3d)
{
	pd3d_ = pd3d;

	HRESULT hr;

	hr = FW1CreateFactory(FW1_VERSION, &m_factory);
	if (FAILED(hr))
		return false;

	hr = m_factory->CreateFontWrapper(pd3d_->GetDevice(), L"Arial", &m_fontwrap);
	if (FAILED(hr))
		return false;

	return true;
}

void graphicFont::Release()
{
	m_fontwrap->Release();
	m_factory->Release();
}

void graphicFont::DrawString(LPCWSTR str, float posX, float posY )
{
	m_fontwrap->DrawString(	pd3d_->GetDevContext(),
							str,// String
							64.0f,// Font size
							posX,// X position
							posY,// Y position
							0xff0099ff,// Text color, 0xAaBbGgRr
							0// Flags	
							);
}


SGAP_END