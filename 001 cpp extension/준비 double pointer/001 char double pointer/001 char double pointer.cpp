#include <stdio.h>
#include <string.h>


void InputName(char **ppname, char* srcName);

void main()
{
	char* pname = nullptr;

	InputName(&pname, "Tiger");
	printf("Name = %s\n", pname);

	delete pname;
}

void InputName(char **ppname, char* srcName)
{
	*ppname = new char[10];
	strcpy(*ppname, srcName);
}