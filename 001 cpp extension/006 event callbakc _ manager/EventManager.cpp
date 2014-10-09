#include "EventManager.h"

EventMgr::EventMgr()
{
}

EventMgr::~EventMgr()
{
}


// �̺�Ʈ �ݹ� ���
void EventMgr::RegisterListener(iCallback* pcb)
{
	eventlist.push_back(pcb);
}

void EventMgr::UnregisterListener(iCallback** ppcb)
{
	// ����Ʈ�� ����ִٸ� , return
	if (eventlist.size() <= 0)
		return;

	for (auto x : eventlist)
	{
		// �ش� �����͸� ã�Ҵٸ� , �����
		if (x == *ppcb)
		{
			eventlist.remove(*ppcb);

			delete *ppcb;
			*ppcb = nullptr;

			return;
		}
	}
}

// �̺�Ʈ �뺸 -> ��ϵ� �̺�Ʈ �����ʵ鿡��
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
	// evenlist ��ü�� �ѹ��� ���鼭 , �޸� ����
	for (auto x : eventlist)
	{
		delete x;
		x = nullptr;
	}

	// ����Ʈ ��ü ����
	eventlist.clear();

}