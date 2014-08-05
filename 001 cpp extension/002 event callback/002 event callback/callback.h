#ifndef _CALLBACK_H_
#define _CALLBACK_H_

class iCallback
{
public:
	virtual ~iCallback()	{}
	virtual void operator()(void* p) = 0;
};


//1. �Ϲ� �Լ� �ݹ�
class CallbackFunction : public iCallback
{
public:
	CallbackFunction(void(*pfunc)(void*))
	{
		func_ = pfunc;
	}

	void operator()(void* p)
	{
		(*func_)(p);
	}

private:
	void (*func_)(void*);
};


//2. Ŭ���� ������ �Լ� �ݹ�
template < typename TC >
class CallbackClass : public iCallback
{
public:
	CallbackClass(TC* inst, void(TC::*pfunc)(void*))
	{
		instance_ = inst;
		pfunc_ = pfunc;
	}

	void operator() (void* pdat)
	{
		(instance_->*pfunc_)(pdat);
	}

private:
	TC*		instance_;
	void	(TC::*pfunc_)(void*);
};

#endif