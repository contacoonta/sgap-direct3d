/*
	Function Pointer : �Լ� ������
*/

#include <iostream>

void show(int a);
void show2(int a);

void main()
{
	// ������ ������ - ������ �ּҸ� ��´�.
	int *pvar = nullptr;
	
	// �Լ��� ������ - �Լ��� �ּҸ� ��´�.
	void (*pfunc)(int) = nullptr;

	// �Լ� �����Ϳ� �Լ� ����
	pfunc = show;
	// �Լ� ��
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