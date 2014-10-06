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
	// 이벤트 콜백 등록
	void operator += (iCallback* pcb)
	{
		RegisterListener(pcb);
	}

	// 이벤트 콜백 개별적으로 지우기
	void operator -= (iCallback** ppcb)
	{
		UnregisterListener(ppcb);
	}

	// 등록된 이벤트 콜백 모두 삭제
	void ClearAllListener();
	// 이벤트 통보 -> 등록된 이벤트 리스너들에게
	void Notify(void* pdat);

private:
	// 이벤트 콜백 등록
	void RegisterListener(iCallback*);
	
	// 이벤트 콜백 개별적으로 지우기
	void UnregisterListener(iCallback**);

private:
	list<iCallback*> eventlist;

};