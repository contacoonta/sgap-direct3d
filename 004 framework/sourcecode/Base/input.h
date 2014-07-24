#pragma once

#include "basedef.h"

SGAP_BEGIN


typedef class iInput
{
public:
			iInput() {}	
	virtual ~iInput() {}
public:
	virtual void initialize() = 0;

	virtual void KeyDown(UINT ukey) = 0;
	virtual void KeyUp(UINT ukey) = 0;
	virtual bool GetKeyDown(UINT ukey) = 0;
}iINPUT;


/*
	키 입력 클래스
*/
typedef class KeyInput : public iInput
{
public:
	static bool CreateInput(iInput** ppinput);
	static void ReleaseInput(iInput** ppinput);

public:
	KeyInput();
	~KeyInput();

	void initialize();

	void KeyDown(UINT ukey);
	void KeyUp(UINT ukey);
	bool GetKeyDown(UINT ukey);

private:
	bool m_keys[256];

}KEYINPUT;

SGAP_END