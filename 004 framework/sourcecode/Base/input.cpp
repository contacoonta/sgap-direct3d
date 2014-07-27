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
	// 모든키를 false 즉, 안 누른 상태로 초기화
	for (bool & x : m_keys)
	{
		x = false;
	}
}


void KeyInput::KeyDown(UINT ukey)
{
	// 키 배열에 키 눌렀음을 저장
	m_keys[ukey] = true;
}

void KeyInput::KeyUp(UINT ukey)
{
	// 키 배열에 키 뗐음을 저장
	m_keys[ukey] = false;
}

bool KeyInput::GetKeyDown(UINT ukey)
{
	// 현재 키 상태 false / true 반환 ( 안눌림 / 눌림 ) 
	return m_keys[ukey];
}

SGAP_END