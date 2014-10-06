/*
	char double pointer
*/

#include <iostream>


void InputName(char** ppname, char* srcname);
void ClearName(char** pname);

void main()
{
	char* pname = nullptr;
	
	/*
		동적 할당 20byte 해서 
		"Jessica" 출력하기
	*/
	/*
	pname = new char[20];
	strcpy(pname, "Jessica");
	std::cout << pname << std::endl;*/

	InputName(&pname, "Jessica");
	std::cout << pname << std::endl;

	//delete [] pname;
	ClearName(&pname);
}

void InputName(char** ppname, char* srcname)
{
	*ppname = new char[20];

	strcpy(*ppname, srcname);
}

void ClearName(char** pname)
{
	delete[] *pname;
	*pname = nullptr;
}