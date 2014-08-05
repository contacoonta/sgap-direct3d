/*
	function pointer
*/

#include <iostream>

void show(int);
void show2(int);

void main()
{
	// 변수형 포인터
	int* ptr = nullptr;

	// 함수형 포인터
	void(*pfunc)(int);

	// call
	pfunc = show;
	pfunc(10);

	// call
	pfunc = show2;
	pfunc(20);
}

void show(int a)
{
	std::cout << "func show" << std::endl;
	std::cout << a << std::endl;
}

void show2(int a)
{
	std::cout << "func show2" << std::endl;
	std::cout << a << std::endl;
}