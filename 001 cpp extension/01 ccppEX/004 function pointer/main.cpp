/*
	Function Pointer : 함수 포인터
*/

#include <iostream>

void show(int a);
void show2(int a);

void main()
{
	// 변수형 포인터 - 변수의 주소를 담는다.
	int *pvar = nullptr;
	
	// 함수형 포인터 - 함수의 주소를 담는다.
	void (*pfunc)(int) = nullptr;

	// 함수 포인터에 함수 전달
	pfunc = show;
	// 함수 콜
	pfunc(30);

	//
	pfunc = show2;
	pfunc(15);
}

void show(int a)
{
	std::cout << "show1 = " << a <<	std::endl;
}

void show2(int a)
{
	std::cout << "show2 = " << a << std::endl;
}