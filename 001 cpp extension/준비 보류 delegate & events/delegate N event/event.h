/*
	Event.h
*/

#ifndef _EVENT_H_
#define _EVENT_H_

#include <list>
#include <vector>
using std::list;
using std::vector;


template<class TSender>
class Event
{
	list< BaseDelegate<TSender>* > m_observers;
	void Register(const BaseDelegate<TSender>* p_handler);
	void Unregister(const BaseDelegate<TSender>* p_handler);
public:
	void operator += (const BaseDelegate<TSender>* p_handler);
	void operator -= (const BaseDelegate<TSender>* p_handler);
	void operator () (const TSender* p_sender, void* p_parameter);
	void Call(const TSender* p_sender, void* p_parameter);
	~Event();
};

template<class TSender>
void Event<TSender>::operator += (const BaseDelegate<TSender>* p_handler)
{
	Register(p_handler);
}

template<class TSender>
void Event<TSender>::operator -= (const BaseDelegate<TSender>* p_handler)
{
	Unregister(p_handler);
}

template<class TSender>
void Event<TSender>::operator ()(const TSender* p_sender, void* p_parameter)
{
	Call(p_sender, p_parameter);
}

template<class TSender>
void Event<TSender>::Register(const BaseDelegate<TSender>* p_handler)
{
	assert(p_handler != NULL);

	for (list< BaseDelegate<TSender>* >::iterator itr = m_observers.begin();
		itr != m_observers.end();
		itr++)
	{
		if ((*itr)->Equals(p_handler))
			return;
	}

	m_observers.push_back(const_cast<BaseDelegate<TSender>*>(p_handler));
}

template<class TSender>
void Event<TSender>::Unregister(const BaseDelegate<TSender>* p_handler)
{
	assert(p_handler != NULL);

	for (list< BaseDelegate<TSender>* >::iterator itr = m_observers.begin();
		itr != m_observers.end();
		itr++)
	{
		if ((*itr)->Equals(p_handler))
		{
			m_observers.erase(itr);
			return;
		}
	}
}


template<class TSender>
void Event<TSender>::Call(const TSender* p_sender, void* p_parameter)
{
	for (list< BaseDelegate<TSender>* >::iterator itr = m_observers.begin();
		itr != m_observers.end();
		itr++)
	{
		//if ( *itr != nullptr )
		(*itr)->Call(p_sender, p_parameter);
	}
}

template<class TSender>
Event<TSender>::~Event()
{
	for (list< BaseDelegate<TSender>* >::iterator itr = m_observers.begin();
		itr != m_observers.end();
		itr++)
	{
		assert(*itr != NULL);
		delete (*itr);
	}
}


#endif