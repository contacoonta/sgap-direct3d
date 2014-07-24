#include <stdio.h>

template < typename T >
void newdata(void** ppdat, T num);

void main()
{
	void* pdat = nullptr;
	newdata(&pdat , 3.14f);

	//*(int*)pdat = 1234;
	//printf("data = %d\n", *(int*)pdat);
	printf("data = %.2f\n", *(float*)pdat);

	delete pdat;
}

template < typename T >
void newdata(void** ppdat, T num)
{
	*ppdat = new T;

	*(T*)*ppdat = num;
}