// ConsoleApplication4.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<Windows.h>
TCHAR *a = TEXT("\\\\.\\asd_zlz1");
TCHAR *b = TEXT("\\\\.\\asd_zlz2");
int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE sd2 = CreateFile(b, GENERIC_ALL, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, NULL);
	printf("%d", GetLastError());
	HANDLE sd1 = CreateFile(a, GENERIC_ALL, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, NULL);
	printf("%d",GetLastError());
	Sleep(1000);
	return 0;
}

