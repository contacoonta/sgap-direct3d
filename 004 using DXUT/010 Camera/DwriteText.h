#pragma once
#include "base.h"
#include <list>
#include <DirectXMath.h>

using std::list;
using std::wstring;
using namespace DirectX;

class DwriteText
{
SINGLETON(DwriteText);

public:
	void	SetHeight(_In_ INT height);
	void	SetMaxLines(_In_ INT maxline);
	void	SetColor(_In_ const XMVECTOR color);		// A B G R
	void	SetFontSize(_In_ FLOAT fontsize);

	// 문자 리스트 출력
	void	AddString(wstring wstr);
	void	RenderStrings();
	
	// 단순 문자 출력
	void	Render(wstring wtext, INT numex, int x, int y);

private:
	HRESULT		Initialize();
	void		Release();

private:
	IFW1Factory*		m_factory = nullptr;
	IFW1FontWrapper*	m_fontwrap = nullptr;

	list<wstring>	m_strings;				// 문자를 넣을 리스트
	INT				m_height	= 18;		// 문자 높이 간격
	INT				m_maxline	= 20;		// 최대 출력 문자열 수
	UINT			m_color = 0xffffffff;	// 글자 색
	FLOAT			m_fontsize = 24.0f;		// 글자 크기

};

#define TEXT DwriteText::instance()