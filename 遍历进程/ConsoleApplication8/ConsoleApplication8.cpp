// ConsoleApplication8.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<Windows.h>
#include<TlHelp32.h>
#include<locale.h>
int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "chs");
	HANDLE hand=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32W info;
	info.dwSize = sizeof(PROCESSENTRY32W);
	Process32First(hand, &info);
	wprintf(TEXT("���̵�����:%s\n"), info.szExeFile);
	wprintf(TEXT("���̵�ID:%u\n"), info.th32ProcessID);
	while (Process32Next(hand, &info))
	{
		wprintf(TEXT("���̵�����:%s\n"), info.szExeFile);
		wprintf(TEXT("���̵�ID:%u\n"), info.th32ProcessID);
	}
	return 0;
}

