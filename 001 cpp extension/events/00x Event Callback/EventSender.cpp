#include "EventSender.h"

void EventSender::RegisterListener(iCallback* pcb)
{
	cbList.push_back(pcb);
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