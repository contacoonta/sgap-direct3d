
#ifndef _CALLBACK_SYSTEM_
#define _CALLBACK_SYSTEM_

class iCallback
{
public:

	virtual ~iCallback() {}
	virtual int operator() (void* args) = 0;
};

//1.�Ϲ� �Լ� �ݹ�
class CallbackFunction : public iCallback
{
public:
	CallbackFunction(int(*func)(void*))
	{
		func_ = func;
	}

	virtual int operator() (void* args) 
	{
		return (*func_)(args); 
	}

private:
	int(*func_)(void*);
};


//2.Ŭ���� ���� �Լ� �ݹ�
template <typename tClass>
class CallbackClass : public iCallback
{
public:

	CallbackClass(tClass* instance, int (tClass::*memberfunction)(void*)) 
	{
		instance_ = instance;
		memberfunc_ = memberfunction;
	}

	virtual int operator() (void* args)
	{ 
		return (instance_->*memberfunc_)(args); 
	}

private:
	tClass* instance_;
	int (tClass::*memberfunc_)(void*);
};

#endif