#pragma once
#include "base.h"
#include <list>

using std::list;
using std::wstring;

class DwriteText
{
SINGLETON(DwriteText);

public:
	// ���� ����Ʈ ���
	void	AddString(wstring wstr);
	void	RenderStrings();
	
	// �ܼ� ���� ���
	void	Render(wstring wtext, INT numex, int x, int y, float fsize = 24.0f);

private:
	HRESULT		Initialize();
	void		Release();

private:
	IFW1Factory*		m_factory = nullptr;
	IFW1FontWrapper*	m_fontwrap = nullptr;

	list<wstring>	m_strings;			// ���ڸ� ���� ����Ʈ
	int				m_height	= 18;	// ���� ���� ����
	int				m_maxline	= 20;	// �ִ� ��� ���ڿ� ��
};

#define TEXT DwriteText::instance()