#pragma once

/*
	인터페이스 클래스
*/
class iCallback
{

public:
			iCallback()			{}
	virtual ~iCallback()		{}

	virtual void operator()(void*)	= 0;

	virtual void RegisterCallback(void(*pfunc)(void*)) = 0;
	virtual void ShootCallback(void* pdat) = 0;
};

/*
	일반 함수 콜백 클래스
*/
class CallbackFunction : public iCallback
{
public:
	
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
	void (*m_pfunc)(void*)		= nullptr;
};