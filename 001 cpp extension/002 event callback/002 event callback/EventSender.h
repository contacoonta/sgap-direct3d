#pragma once

#include <list>
#include "callback.h"

using std::list;

class EventSender
{
public:
	void RegisterListener(iCallback*);
	void UnregisterListener(iCallback**);
	
	void Invoke(void* pdat);
	
	void operator += (iCallback* pcb)
	{
		RegisterListener(pcb);
	}

	void operator -= (iCallback** ppcb)
	{
		UnregisterListener(ppcb);
	}

private:
	list<iCallback*> cbList;
};
