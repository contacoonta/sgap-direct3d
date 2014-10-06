#include <stdio.h>

template < typename V >
void CreateData(void** ppdat, V val);
void ReleaseData(void** ppdat);

void main()
{
	void* pdat = nullptr;

	/*
		pdat 에 정수 26 넣고 출력
	*/
	/*pdat = new int(26);
	{
		printf("data = %d\n", *(int*)pdat );
	}
	delete pdat;
	pdat = nullptr;*/

	CreateData(&pdat, 45.45f);
	printf("data = %f\n", *(float*)pdat);
	ReleaseData(&pdat);
}

template < typename V >
void CreateData(void** ppdat, V val)
{
	//*ppdat = new int(val);
	*ppdat = new V;
	*(V*)*ppdat = val;
}

void ReleaseData(void** ppdat)
{
	if (*ppdat == nullptr)
		return;

	delete *ppdat;
	*ppdat = nullptr;
}