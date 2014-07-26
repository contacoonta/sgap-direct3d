#include <iostream>

using std::cout;
using std::endl;

template < typename T >
void CreateData(void ** ppdat, T val);
void ReleaseData(void ** ppdat);

void main()
{
	void* pdat = nullptr;

	// 1. main �Լ����� �Ϲ������� pdat �����͸� ����� ���.
	/*pdat = new int(77);
	{
		printf("data = %d\n", *(int*)pdat);
	}
	delete pdat;*/

	// 2. �Լ� ���ڷ� pdat �������� �ּҸ� �����Ͽ� ����� ���.
	CreateData(&pdat, 'k');
	{
		//cout << "data = " << *(int*)pdat << endl;
		cout << "data = " << *(char*)pdat << endl;
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