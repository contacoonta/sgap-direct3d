#include <iostream>
#include "callback.h"


void OnEventShow(void* pdat);
void OnEventDie(void* pdat);

void main()
{
	iCallback* pcallback = new CallbackFunction;

	// ���� ����ȯ - ������ ����ȯ -> ��� ������
	/*CallbackFunction* pcf = (CallbackFunction*)(pcallback);
	CallbackFunction* pcf = static_cast<CallbackFunction*>(pcallback);*/
	
	// ���� ����ȯ - ���� ����ȯ -> ��� ���� �ϸ� ������, ���� nullptr
	CallbackFunction* pcf = dynamic_cast<CallbackFunction*>(pcallback);
	if (pcf == nullptr)
	{
		delete pcallback;
		return;
	}

	// �ܺ� �Լ� ���
	pcf->RegisterCallback(OnEventShow);
	// �Լ� ��
	int val = 10;	
	
	// ���ǵ� �Լ� ��
	pcf->ShootCallback(&val);
	// Operator �� �������� �Լ� ��
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