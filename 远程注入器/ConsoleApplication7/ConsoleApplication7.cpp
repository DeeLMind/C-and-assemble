// ConsoleApplication7.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<Windows.h>
#include<TlHelp32.h>
#include<locale.h>
TCHAR *buf = (TCHAR*)malloc(0);
int _tmain(int argc, _TCHAR* argv[])
{
	SIZE_T a;
	HANDLE took;
	LUID PD; 
	DWORD processId=-1;
	int i = 5;
	setlocale(LC_ALL, "chs");
	printf("��������Ҫע��Ľ�����:");
	wscanf_s(TEXT("%s"), buf, 20);
    HANDLE hand = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32W info;
	info.dwSize = sizeof(PROCESSENTRY32W);
	Process32First(hand, &info);
	if (wcscmp(info.szExeFile, buf)==0)
	{
		processId = info.th32ProcessID;
	}
	else
	{
		while (Process32Next(hand, &info))
		{
			if (wcscmp(info.szExeFile, buf)==0)
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
	wprintf(TEXT("׼��ע��"));
	while (i)
	{
		Sleep(0000);
		printf(".");
		i--;
	}
	CHAR *b = "C:\\hookbyzlz.dll";
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &took);
	TOKEN_PRIVILEGES tp;
	tp.PrivilegeCount = 1;
	tp.Privileges->Attributes = SE_PRIVILEGE_ENABLED;
	LookupPrivilegeValue(NULL, TEXT("SeDebugPrivilege"), &PD);
	tp.Privileges->Luid = PD;
	AdjustTokenPrivileges(took, FALSE, &tp, NULL, NULL, NULL);
	HANDLE exploer=OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	PTHREAD_START_ROUTINE pfn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "LoadLibraryA");
	void* add = VirtualAllocEx(exploer, NULL, strlen(b), MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(exploer, add, b, strlen(b),&a);
	HANDLE thread=CreateRemoteThread(exploer,NULL, 0, pfn, add, 0, NULL);
	if (GetLastError() == ERROR_SUCCESS)
	{
		printf("ע��ɹ�!\n");
	}
	else
	{
		printf("ע��ʧ��,����Ҫ�Թ���Ա������г�����Թر�360\n");
	}
	return 0;
}

