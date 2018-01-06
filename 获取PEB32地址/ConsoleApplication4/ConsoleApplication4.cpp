// ConsoleApplication4.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<Windows.h>
#include<TlHelp32.h>
#include "ntdll_undoc.h"
#pragma comment(lib,"ntdll.lib")

TCHAR buf[MAX_PATH] = { 0 };

int main()
{
	DWORD processId = -1;
	ULONG_PTR Peb32 = NULL;
	DWORD ByteRet = 0;
	printf("������:");
	wscanf_s(TEXT("%s"), buf, 20);
	HANDLE hand = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32W info;
	info.dwSize = sizeof(PROCESSENTRY32W);
	Process32First(hand, &info);
	if (wcscmp(info.szExeFile, buf) == 0)
	{
		processId = info.th32ProcessID;
	}
	else
	{
		while (Process32Next(hand, &info))
		{
			if (wcscmp(info.szExeFile, buf) == 0)
			{
				processId = info.th32ProcessID;
				break;
			}
		}
	}
	if (processId == -1)
	{
		printf("ʧ�ܣ��Ҳ���ָ����ģ��\n");
		return 0;
	}
	wprintf(TEXT("������:%s\n"), buf);
	wprintf(TEXT("����ID:%d\n"), processId);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    //����LdrQueryProcessPeb32
	NtQueryInformationProcess(hProcess, ProcessWow64Information, &Peb32, sizeof(ULONG_PTR), &ByteRet);
	printf("PEB32:%p", (void*)Peb32);
	Sleep(INFINITE);
    return 0;
}

