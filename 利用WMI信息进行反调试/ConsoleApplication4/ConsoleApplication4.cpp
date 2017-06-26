// ConsoleApplication4.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <atlstr.h> 
#include <atlconv.h>
#include<comutil.h>
#include<iostream>
#include<string>
#include<WbemIdl.h>
#include<objbase.h>
using namespace std;
using std::string;
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "comsuppw.lib")
BOOL ManageWMIInfo(string &result, string table, wstring wcol)
{
	HRESULT hres;
	char bord[1024];
	//��ʼ��COM   
	hres = CoInitialize(0);
	//���WMI����COM�ӿ�    
	IWbemLocator *pLoc = NULL;
	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *)&pLoc);
	if (FAILED(hres))
	{
		cout << "Failed to create IWbemLocator object."
			<< "Err code = 0x"
			<< hex << hres << endl;
		CoUninitialize();
		return false;
	}
	//ͨ�����ӽӿ�����WMI���ں˶�����ROOT//CIMV2    
	IWbemServices *pSvc = NULL;
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace  
		NULL, // User name. NULL = current user  
		NULL, // User password. NULL = current  
		0, // Locale. NULL indicates current  
		NULL, // Security flags.  
		0, // Authority (e.g. Kerberos)  
		0, // Context object   
		&pSvc // pointer to IWbemServices proxy  
	);
	if (FAILED(hres))
	{
		cout << "Could not connect. Error code = 0x"
			<< hex << hres << endl;
		pLoc->Release();
		CoUninitialize();
		return false;
	}
	//�����������İ�ȫ����     
	hres = CoSetProxyBlanket(
		pSvc, // Indicates the proxy to set  
		RPC_C_AUTHN_WINNT, // RPC_C_AUTHN_xxx  
		RPC_C_AUTHZ_NONE, // RPC_C_AUTHZ_xxx  
		NULL, // Server principal name   
		RPC_C_AUTHN_LEVEL_CALL, // RPC_C_AUTHN_LEVEL_xxx   
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx  
		NULL, // client identity  
		EOAC_NONE // proxy capabilities   
	);
	if (FAILED(hres))
	{
		cout << "Could not set proxy blanket. Error code = 0x"
			<< hex << hres << endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return false;
	}
	//ͨ�������������WMI��������  
	IEnumWbemClassObject* pEnumerator = NULL;
	string select = "SELECT * FROM " + table;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t(select.c_str()),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);
	if (FAILED(hres))
	{
		cout << "Query for Network Adapter Configuration failed."
			<< " Error code = 0x��"
			<< hex << hres << endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return false;
	}
	//ѭ��ö�����еĽ������  
	ULONG uReturn = 0;
	IWbemClassObject *pclsObj=NULL;
	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
			&pclsObj, &uReturn);
		if (0 == uReturn)
		{
			break;
		}
		VARIANT vtProp;
		VariantInit(&vtProp);
		hr = pclsObj->Get(wcol.c_str(), 0, &vtProp, 0, 0);
		if (!FAILED(hr))
		{
			CW2A tmpstr1(vtProp.bstrVal);
			strcpy_s(bord, 200, tmpstr1);
			cout << bord << endl;
			result = bord;
		}
		VariantClear(&vtProp);
	}
	//�ͷ���Դ    
	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	pclsObj->Release();
	CoUninitialize();
	return true;
}
BOOL CheckVirtualBox()
{
	string table = "Win32_computersystem";
	wstring wcol = L"Model";
	string ret;
	ManageWMIInfo(ret, table, wcol);
	if (ret.find("VirtualBox") != string::npos)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CheckVMWare()
{
	string table = "Win32_DiskDrive";
	wstring wcol = L"Caption";
	string ret;
	ManageWMIInfo(ret, table, wcol);
	if (ret.find("VMware") != string::npos)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
int main()
{
	if (CheckVMWare() || CheckVirtualBox())
	{
		cout << "��⵽VMWARE��VIRTUALBOX�����" << endl;
	}
	else
	{
		cout << "û�м�⵽�����" << endl;
	}
	Sleep(INFINITE);
    return 0;
}

