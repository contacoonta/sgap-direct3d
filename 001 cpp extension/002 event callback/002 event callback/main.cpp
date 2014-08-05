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
	//sender.RegisterListener(event1);
	//sender.RegisterListener(event2);

	//오퍼레이터로 이벤트 등록
	sender += event1;
	sender += event2;
	
	data dat;
	dat.posx = 3;
	dat.posy = 14;
	sender.Invoke(&dat);

	std::cout << "------------------" << std::endl;

	//오퍼레이터로 이벤트 삭제
	sender -= &event1;
	sender.Invoke(&dat);

	//삭제한 이벤트는 생략
	if (event1)
	{
		delete event1;
		event1 = nullptr;
	}

	if (event2)
	{
		delete event2;
		event2 = nullptr;
	}

	return 0;
}
