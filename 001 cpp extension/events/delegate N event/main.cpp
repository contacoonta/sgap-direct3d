
#include <iostream>
#include "delegate.h"
#include "event.h"

using std::cout;
using std::endl;


/*
	하나의 Event Manager
*/
#define EVENTMGR EventMgr::getinstance()
class EventMgr
{
public:
	static EventMgr & getinstance()
	{
		static EventMgr instance;
		return instance;
	}

public:
	Event<EventMgr> Spawn;
	void PlayerSpawn(char* p_msg)	{	Spawn(this, p_msg);		}

	Event<EventMgr> Die;
	void PlayerDie(char* pmsg)		{	Die(this, pmsg);		}
};


/*
	여러개의 Event Listener
*/
static int s_id = 0;
class Listener
{
	int m_id;
	BaseDelegate<EventMgr>* pEventSpawn;
	BaseDelegate<EventMgr>* pEventDie;

	void OnSpawn(const EventMgr* p_EventMgr, void* p_parameter)
	{
		cout << "Listener.." << m_id << " ] " << (char*)p_parameter << endl;
	}

	void OnDie(const EventMgr* p_EventMgr, void* p_parameter)
	{
		cout << "Listener.." << m_id << " ] " << (char*)p_parameter << endl;
	}


public:
	Listener()
	{
		m_id = ++s_id;

		pEventSpawn = new Delegate<Listener, EventMgr>(this, &Listener::OnSpawn);
		EVENTMGR.Spawn += pEventSpawn;
		
		pEventDie = new Delegate<Listener, EventMgr>(this, &Listener::OnDie);
		EVENTMGR.Die += pEventDie;
	}

	~Listener()
	{
		EVENTMGR.Spawn -= pEventSpawn;
		delete pEventSpawn;

		EVENTMGR.Die -= pEventDie;
		delete pEventDie;
	}
	
};



int main()
{
	Listener listener[5];
	Listener * plistener = new Listener();

	cout << "-------------------------" << endl;
	EVENTMGR.PlayerSpawn("Player SPAWN..");
	
	cout << "-------------------------" << endl;
	EVENTMGR.PlayerDie("Player DIE ..");


	delete plistener;
	cout << "-------------------------" << endl;
	EVENTMGR.PlayerSpawn("Player RESPAWN..");

	return 0;
}

