#include <stdio.h>

template < typename T >
void CreateData(void ** ppdat, T val);
void ReleaseData(void ** ppdat);

void main()
{
	void* pdat = nullptr;

	/*pdat = new int(77);
	{
		printf("data = %d\n", *(int*)pdat);
	}
	delete pdat;*/

	CreateData(&pdat, 77);
	{
		printf("data = %d\n", *(int*)pdat);
	}
	ReleaseData(&pdat);


	//CreateData(&pdat, 3.14f);
	////CreateData(pdat, 77);

	//printf("data = %d\n", pdat);
	//or
	//printf("data = %f\n", pdat);

	//ReleaseData(&pdat);
}

template < typename T >
void CreateData(void ** ppdat, T val)
{
	*ppdat = new T;

	*(T*)*ppdat = val;
}

void ReleaseData(void ** ppdat)
{
	delete *ppdat;
}