#include "input.h"


bool input::CreateInput(input** ppinput)
{
	if (*ppinput != nullptr)
		return false;
	
	*ppinput = new input;
	(*ppinput)->initialize();	

	return true;
}

void input::ReleaseInput(input** ppinput)
{
	if (*ppinput)
	{
		delete *ppinput;
		*ppinput = nullptr;
	}
}


input::input()
{
}

input::input(const input& i)
{
}

input::~input()
{
}


void input::initialize()
{
	// ���Ű�� false ��, �� ���� ���·� �ʱ�ȭ
	for (bool & x : m_keys)
	{
		x = false;
	}
}


void input::KeyDown(UINT ukey)
{
	// Ű �迭�� ���� ���·� ����
	m_keys[ukey] = true;
}

void input::KeyUp(UINT ukey)
{
	// Ű �迭�� �� ���·� ����
	m_keys[ukey] = false;
}

bool input::GetKeyDown(UINT ukey)
{
	// ���� Ű ���� false / true ��ȯ ( �ȴ��� / ���� ) 
	return m_keys[ukey];
}