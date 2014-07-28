#include "EventSender.h"

void EventSender::RegisterListener(iCallback* pcb)
{
	cbList.push_back(pcb);
}

void EventSender::UnregisterListener(iCallback** ppcb)
{
	for (auto* x : cbList)
	{
		if (x == *ppcb)
		{
			cbList.remove(*ppcb);

			delete *ppcb;
			*ppcb = nullptr;

			return;
		}
	}
}

void EventSender::Invoke(void * pdat)
{
	if (cbList.empty())
		return;

	for (auto x : cbList )
	{
		(*x)(pdat);
	}
}