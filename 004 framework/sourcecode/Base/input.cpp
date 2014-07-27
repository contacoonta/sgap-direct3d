#include "input.h"

SGAP_BEGIN

bool KeyInput::CreateInput(iInput** ppinput)
{
	if (*ppinput != nullptr)
		return false;

	*ppinput = new KeyInput;
	(*ppinput)->initialize();	

	return true;
}

void KeyInput::DestroyInput(iInput** ppinput)
{
	if (*ppinput)
	{
		delete *ppinput;
		*ppinput = nullptr;
	}
}


KeyInput::KeyInput()
{
}

KeyInput::KeyInput(const KeyInput&)
{
}

KeyInput::~KeyInput()
{
}


void KeyInput::initialize()
{
	// ���Ű�� false ��, �� ���� ���·� �ʱ�ȭ
	for (bool & x : m_keys)
	{
		x = false;
	}
}


void KeyInput::KeyDown(UINT ukey)
{
	// Ű �迭�� Ű �������� ����
	m_keys[ukey] = true;
}

void KeyInput::KeyUp(UINT ukey)
{
	// Ű �迭�� Ű ������ ����
	m_keys[ukey] = false;
}

bool KeyInput::GetKeyDown(UINT ukey)
{
	// ���� Ű ���� false / true ��ȯ ( �ȴ��� / ���� ) 
	return m_keys[ukey];
}

SGAP_END