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

	// ���� ����Ʈ ���
	void	AddString(wstring wstr);
	void	RenderStrings();
	
	// �ܼ� ���� ���
	void	Render(wstring wtext, INT numex, int x, int y);

private:
	HRESULT		Initialize();
	void		Release();

private:
	IFW1Factory*		m_factory = nullptr;
	IFW1FontWrapper*	m_fontwrap = nullptr;

	list<wstring>	m_strings;				// ���ڸ� ���� ����Ʈ
	INT				m_height	= 18;		// ���� ���� ����
	INT				m_maxline	= 20;		// �ִ� ��� ���ڿ� ��
	UINT			m_color = 0xffffffff;	// ���� ��
	FLOAT			m_fontsize = 24.0f;		// ���� ũ��

};

#define TEXT DwriteText::instance()