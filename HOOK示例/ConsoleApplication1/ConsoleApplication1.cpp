// ConsoleApplication1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<Windows.h>
#include "Win32Project2.h"
#pragma comment(lib,"Win32Project2.lib")
int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE ev = CreateEvent(NULL, FALSE, FALSE, NULL);
	instailhook(NULL);
	WaitForSingleObject(ev, INFINITE);
}