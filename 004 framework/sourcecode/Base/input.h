#ifndef _INPUT_H_
#define _INPUT_H_

#include "basedef.h"

SGAP_BEGIN


typedef class iInput
{
public:
			iInput() {}	
			iInput(const iInput&) {}
	virtual ~iInput() {}
public:
	virtual void initialize() = 0;

	virtual void KeyDown(UINT ukey) = 0;
	virtual void KeyUp(UINT ukey) = 0;
	virtual bool GetKeyDown(UINT ukey) = 0;
}iINPUT, *LPINPUT;


/*
	윈도 메시지 기반 키 입력 클래스
*/
typedef class KeyInput : public iInput
{
public:
	static bool Create(iInput** ppinput);
	static void Destroy(iInput** ppinput);

public:
	KeyInput();
	KeyInput(const KeyInput&);
	~KeyInput();

	void initialize();

	void KeyDown(UINT ukey);
	void KeyUp(UINT ukey);
	bool GetKeyDown(UINT ukey);

private:
	bool m_keys[256];

}KEYINPUT;

SGAP_END

#endif