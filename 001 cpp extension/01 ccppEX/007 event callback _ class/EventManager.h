#pragma once

#include <list>
#include <string>
#include "callback.h"

using std::list;
using std::string;


typedef enum EventType
{
	NONE,
	SPAWN,
	LEVELUP,
	DAMAGE,
	KILL,

}EVENTTYPE;

// Event Packet Type
typedef struct EventData
{
	EVENTTYPE etype = NONE;
	
	int posx = 0;
	int posy = 0;
	string strname = "";

}EVENTDATA;

//typedef struct EventSpawn
//{
//	int posx = 0;
//	int posy = 0;
//	string strname = "";
//
//}EVENTSPAWN;
//
//typedef struct EventDamage
//{
//	int damage = 0;
//	
//	string srcname = "";
//	string targetname = "";
//
//}EVENTDAMAGE;



class EventMgr
{
public:
	EventMgr();
	~EventMgr();

public:
	// �̺�Ʈ �ݹ� ���
	void operator += (iCallback* pcb)
	{
		RegisterListener(pcb);
	}

	// �̺�Ʈ �ݹ� ���������� �����
	void operator -= (iCallback** ppcb)
	{
		UnregisterListener(ppcb);
	}

	// ��ϵ� �̺�Ʈ �ݹ� ��� ����
	void ClearAllListener();
	// �̺�Ʈ �뺸 -> ��ϵ� �̺�Ʈ �����ʵ鿡��
	void Notify(void* pdat);

private:
	// �̺�Ʈ �ݹ� ���
	void RegisterListener(iCallback*);
	
	// �̺�Ʈ �ݹ� ���������� �����
	void UnregisterListener(iCallback**);

private:
	list<iCallback*> eventlist;

};