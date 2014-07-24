
#ifndef _EVENTSENDER_H_
#define _EVENTSENDER_H_

#include <list>
#include "Callback.h"

using std::list;

class EventSender
{
public:
	void RegisterListener(iCallback*);

	void Invoke(void * pdat);

private:
	list<iCallback*> cbList;
};


#endif