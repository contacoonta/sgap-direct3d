#pragma once

#include <list>
#include "callback.h"

using std::list;

class EventSender
{
public:
	void RegisterListener(iCallback*);
	void Invoke(void* pdat);

private:
	list<iCallback*> cbList;
};