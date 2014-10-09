#pragma once

/*
	�������̽� Ŭ����
*/
class iCallback
{

public:
			iCallback()						{}
			iCallback(void(*pfunc)(void* ))	{}
	virtual ~iCallback()					{}

	virtual void operator()(void*)	= 0;

	virtual void RegisterCallback(void(*pfunc)(void*)) = 0;
	virtual void ShootCallback(void* pdat) = 0;

protected:
	void	(*m_pfunc)(void*) = nullptr;
};

/*
	�Ϲ� �Լ� �ݹ� Ŭ����
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
};