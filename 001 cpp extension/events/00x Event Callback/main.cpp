#include <iostream>
#include "Callback.h"
#include "EventSender.h"


class data
{
public:
	int posx = 10;
	int posy = 20;
};

class Foo
{
public:

	int OnEvent(void* args)
	{
		data* pdat = (data*)args;
		std::cout << "----------class callback------------" << std::endl;
		std::cout << pdat->posx << " , " << pdat->posy << '\n';
		return 2;
	}
};

int OnEvent(void* args)
{
	data* pdat = (data*)args;
	std::cout << "----------function callback------------" << std::endl;
	std::cout << pdat->posx << " , " << pdat->posy << '\n';
	return 2;
}

int main(int argc, char* argv[])
{
	// 클래스 형태 _ callback
	Foo myfoo;
	iCallback* event1 = new CallbackClass<Foo>(&myfoo, &Foo::OnEvent);

	// 함수형태 _ callback
	iCallback* event2 = new CallbackFunction(OnEvent);
	//(*event3)(&dat);
	
	
	EventSender sender;
	sender.RegisterListener(event1);
	sender.RegisterListener(event2);
	
	data dat;
	dat.posx = 3;
	dat.posy = 14;
	sender.Invoke(&dat);

	delete event1;
	delete event2;

	return 0;
}