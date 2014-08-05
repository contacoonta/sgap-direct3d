#include <iostream>

#include "callback.h"
#include "EventSender.h"


typedef struct data
{
	int index = 0;
	int posX = 0;
	int posY = 0;
}DATA;


//1. 함수 이벤트 콜
void OnEvent( void* pdat )
{
	std::cout << "Event Call 1" << std::endl;

	data* pdata = (data*)pdat;

	std::cout << pdata->index << std::endl
				<< pdata->posX << " , " 
				<< pdata->posY << std::endl;
}


//2. 클래스 내부 함수 이벤트 콜
typedef class Listener
{
public:
	void OnEvent(void* pdat)
	{
		std::cout << "Event Call 2" << std::endl;

		data* pdata = (data*)pdat;

		std::cout << pdata->index << std::endl
			<< pdata->posX << " , "
			<< pdata->posY << std::endl;
	}
}LISTENER, *PLISTENER;

int main()
{
	EventSender sender;
	Listener mylistener;
	
	//PLISTENER pListner;
	
	iCallback* event1 = new CallbackFunction(OnEvent);
	iCallback* event2 = new CallbackClass<Listener>(&mylistener, &Listener::OnEvent);

	sender.RegisterListener(event1);
	sender.RegisterListener(event2);

	sender += event1;
	
	DATA dat1;
	dat1.index = 1;
	dat1.posX = 50;
	dat1.posY = -30;

	sender.Invoke(&dat1);


	sender -= event1;

	sender.Invoke(&dat1);


	
	delete event1;
	delete event2;

	return 0;
}