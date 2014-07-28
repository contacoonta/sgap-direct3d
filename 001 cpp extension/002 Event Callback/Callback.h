
#ifndef _CALLBACK_SYSTEM_
#define _CALLBACK_SYSTEM_

class iCallback
{
public:

	virtual ~iCallback() {}
	virtual int operator() (void* args) = 0;
};

//1.일반 함수 콜백
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


//2.클래스 내의 함수 콜백
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