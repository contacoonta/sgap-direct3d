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
	// 모든키를 false 즉, 안 누른 상태로 초기화
	for (bool & x : m_keys)
	{
		x = false;
	}
}


void input::KeyDown(UINT ukey)
{
	// 키 배열에 누른 상태로 저장
	m_keys[ukey] = true;
}

void input::KeyUp(UINT ukey)
{
	// 키 배열에 뗀 상태로 저장
	m_keys[ukey] = false;
}

bool input::GetKeyDown(UINT ukey)
{
	// 현재 키 상태 false / true 반환 ( 안눌림 / 눌림 ) 
	return m_keys[ukey];
}