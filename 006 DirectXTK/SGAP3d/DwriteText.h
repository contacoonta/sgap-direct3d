#pragma once

class DwriteText
{

public:
	DwriteText();
	~DwriteText();

public:
	void			Render(std::wstring wtext, INT numex, int x, int y, float fsize = 24.0f);

private:
	HRESULT			Initialize();
	void			Release();

private:
	IFW1Factory*		m_factory = nullptr;
	IFW1FontWrapper*	m_fontwrap = nullptr;

};

