#include "EventManager.h"

EventMgr::EventMgr()
{
}

EventMgr::~EventMgr()
{
}


// 이벤트 콜백 등록
void EventMgr::RegisterListener(iCallback* pcb)
{
	eventlist.push_back(pcb);
}

void EventMgr::UnregisterListener(iCallback** ppcb)
{
	// 리스트가 비어있다면 , return
	if (eventlist.size() <= 0)
		return;

	for (auto x : eventlist)
	{
		// 해당 포인터를 찾았다면 , 지우기
		if (x == *ppcb)
		{
			eventlist.remove(*ppcb);

			delete *ppcb;
			*ppcb = nullptr;

			return;
		}
	}
}

// 이벤트 통보 -> 등록된 이벤트 리스너들에게
void EventMgr::Notify(void* pdat)
{
	// range based for
	for ( auto o : eventlist )
	{
		o->ShootCallback(pdat);
	}
}

void EventMgr::ClearAllListener()
{
	// evenlist 전체를 한바퀴 돌면서 , 메모리 삭제
	for (auto x : eventlist)
	{
		delete x;
		x = nullptr;
	}

	// 리스트 자체 삭제
	eventlist.clear();

}