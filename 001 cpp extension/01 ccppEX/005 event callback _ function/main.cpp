#include <iostream>
#include "callback.h"


void OnEventShow(void* pdat);
void OnEventDie(void* pdat);

void main()
{
	iCallback* pcallback = new CallbackFunction;

	// 강제 형변환 - 무조건 형변환 -> 결과 포인터
	/*CallbackFunction* pcf = (CallbackFunction*)(pcallback);
	CallbackFunction* pcf = static_cast<CallbackFunction*>(pcallback);*/
	
	// 동적 형변환 - 조건 형변환 -> 결과 성공 하면 포인터, 실패 nullptr
	CallbackFunction* pcf = dynamic_cast<CallbackFunction*>(pcallback);
	if (pcf == nullptr)
	{
		delete pcallback;
		return;
	}

	// 외부 함수 등록
	pcf->RegisterCallback(OnEventShow);
	// 함수 콜
	int val = 10;	
	
	// 정의된 함수 콜
	pcf->ShootCallback(&val);
	// Operator 로 재정의한 함수 콜
	//(*pcf)(&val);
	
	pcf->RegisterCallback(OnEventDie);
	pcf->ShootCallback("M Jackson");
	//(*pcf)("leonardo");
		

	delete pcallback;
}


void OnEventShow(void* pdat)
{
	std::cout << "OnEventShow ] call = " 
		<< *(int*)pdat << std::endl;
}

void OnEventDie(void* pdat)
{
	std::cout << "OnEventShow ] call 2 = "
		<< (char*)pdat << std::endl;
}