#include <iostream>
#include "callback.h"
#include "EventManager.h"

void OnEventSpawn(void* pdat);
void OnEventDie(void* pdat);
void OnEventSpawn2(void* pdat);

class Character
{
public:
	void OnEventSpawn3(void* pdat)
	{
		//Spawn ���� �̺�Ʈ�� ó��
		if (pdat == nullptr)
			return;

		EVENTDATA* evdat = (EVENTDATA*)pdat;
		if (evdat->etype != SPAWN)
			return;

		std::cout << "OnEventSpawn CLASS 3]" << std::endl
					<< evdat->posx << " , " << evdat->posy << std::endl
					<< evdat->strname << std::endl;
	}
};


void main()
{
	EventMgr eventMgr;
	
	/*
		�̺�Ʈ ���
	*/
	iCallback* pevent = new CallbackFunction(OnEventSpawn);
	//eventMgr.RegisterListener(pevent);
	eventMgr += pevent;

	iCallback* pevent2 = new CallbackFunction(OnEventDie);
	//eventMgr.RegisterListener(pevent2);
	eventMgr += pevent2;

	iCallback* pevent3 = new CallbackFunction(OnEventSpawn2);
	//eventMgr.RegisterListener(pevent3);
	eventMgr += pevent3;

	Character jessica;
	iCallback* pevent4 = new CallbackClass<Character>(&jessica, &Character::OnEventSpawn3);
	eventMgr += pevent4;
	

	/*
		�̺�Ʈ ��Ŷ�� ����� ���
	*/
	EVENTDATA dat;
	dat.etype = SPAWN;
	dat.posx = 10;
	dat.posy = 20;
	dat.strname = "KJE";
	eventMgr.Notify(&dat);

	/*
		�̺�Ʈ ����
	*/
	//eventMgr.UnregisterListener(&pevent);
	eventMgr -= &pevent;
	

	dat.etype = KILL;
	dat.posx = 20;
	dat.posy = 30;
	dat.strname = "MJackson";
	eventMgr.Notify(&dat);
	
	/*
		��� �̺�Ʈ ����
	*/
	eventMgr.ClearAllListener();
}


void OnEventSpawn(void* pdat)
{
	//Spawn ���� �̺�Ʈ�� ó��
	if (pdat == nullptr )
		return;

	EVENTDATA* evdat = (EVENTDATA*)pdat;
	if (evdat->etype != SPAWN)
		return;

	std::cout << "OnEventSpawn 1]" << std::endl
			<< evdat->posx << " , " << evdat->posy << std::endl
			<< evdat->strname << std::endl;
}

void OnEventDie(void* pdat)
{
	//Die ���� �̺�Ʈ�� ó��
	if (pdat == nullptr)
		return;

	EVENTDATA* evdat = (EVENTDATA*)pdat;
	if (evdat->etype != KILL)
		return;

	std::cout << "OnEventKILL ]" << std::endl
		<< evdat->posx << " , " << evdat->posy << std::endl
		<< evdat->strname << std::endl;
}

void OnEventSpawn2(void* pdat)
{
	//Spawn ���� �̺�Ʈ�� ó��
	if (pdat == nullptr)
		return;

	EVENTDATA* evdat = (EVENTDATA*)pdat;
	if (evdat->etype != SPAWN)
		return;

	std::cout << "OnEventSpawn 2]" << std::endl
		<< evdat->posx << " , " << evdat->posy << std::endl
		<< evdat->strname << std::endl;
}