// hookbyzlz.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "hookbyzlz.h"
#include<Windows.h>

// ���ǵ���������һ��ʾ����
HOOKBYZLZ_API int fnhookbyzlz()
{
	MessageBox(NULL, TEXT("ע��ɹ�"), NULL, MB_OK);
	return 0;
}

