#pragma once

/*
	인터페이스 클래스
*/
class iCallback
{
public:
	virtual ~iCallback()					{}
	virtual void operator()(void*)			= 0;
};

/*
	일반 함수 콜백 클래스
*/
class CallbackFunction : public iCallback
{
public:	
	CallbackFunction(void(*pfunc)(void*))	{ m_pfunc = pfunc; }

	void RegisterCallback(void(*pfunc)(void*))
	{
		m_pfunc = pfunc;
	}

	void ShootCallback(void* pdat)
	{
		m_pfunc(pdat);
	}

	void operator()(void* pdat)
	{
		m_pfunc(pdat);
		//ShootCallback(pdat);
	}

private:
	void(*m_pfunc)(void*) = nullptr;
};

/*
	클래스 안의 함수 콜백
*/
template < typename TC >
class CallbackClass : public iCallback
{
public:
	CallbackClass(TC* inst, void(TC::*pfunc)(void*))
	{
		instance_ = inst;
		m_pfunc = pfunc;
	}

	void operator()(void* pdat)
	{
		if (instance_ == nullptr)
			return;

		(instance_->*m_pfunc)(pdat);
	}

private:
	TC*		instance_ = nullptr;
	void	(TC::*m_pfunc)(void*) = nullptr;
};