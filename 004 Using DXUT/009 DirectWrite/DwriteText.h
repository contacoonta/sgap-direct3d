#pragma once
#include "base.h"
#include <list>

using std::list;
using std::wstring;

class DwriteText
{
SINGLETON(DwriteText);

public:
	// 문자 리스트 출력
	void	AddString(wstring wstr);
	void	RenderStrings();
	
	// 단순 문자 출력
	void	Render(wstring wtext, INT numex, int x, int y, float fsize = 24.0f);

private:
	HRESULT		Initialize();
	void		Release();

private:
	IFW1Factory*		m_factory = nullptr;
	IFW1FontWrapper*	m_fontwrap = nullptr;

	list<wstring>	m_strings;			// 문자를 넣을 리스트
	int				m_height	= 18;	// 문자 높이 간격
	int				m_maxline	= 20;	// 최대 출력 문자열 수
};

#define TEXT DwriteText::instance()