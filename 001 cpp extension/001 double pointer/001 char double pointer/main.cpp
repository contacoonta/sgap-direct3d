/*
	char double pointer
*/
#include <iostream>

void InputName(char ** ppname, char * srcname);

void main()
{
	char* pname = nullptr;

	/*pname = new char[10];
	strcpy(pname, "TigerMask");
	std::cout << pname << std::endl;*/

	InputName(&pname, "TigerMask");
	std::cout << pname << std::endl;

	delete pname;
}

void InputName(char ** ppname, char * srcname)
{
	*ppname = new char[10];
	strcpy(*ppname, srcname);
}