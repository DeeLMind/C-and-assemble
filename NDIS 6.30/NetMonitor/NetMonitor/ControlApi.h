#pragma once
#include"global.h"
#include"Winproc.h"
#include"..\..\RawPacketAnalysis\PacketInfo.h"
#define ARPPACKETLENGTH 60
extern map<int, PacketInfo> ListInfo;
typedef struct _ListViewInfo
{
	char TimeOrIndex[20];
	char SourceIp[20];
	char DestIp[20];
	char DataLength[8];
	char ProtocolType[8];
	char Information[255];
}ListViewInfo, *PListViewInfo;
void InitListInfo(PacketInfo Info, ListViewInfo *ListViewInfo);
VOID CALLBACK GetRawPacket(HWND hwnd, UINT msg, UINT_PTR timeid, DWORD systemtime);
void StartFilter(HWND hDlg);
void StopFilter(HWND hDlg);
void FindCard(HWND hDlg);
void DeleteAllListInfo();
void Refresh();
void ShowOutput(LPNMLISTVIEW Mlv);              //Static�ؼ���ʾ��ϸ��Ϣ
void AddListView(PListViewInfo Info);
void ChangeListInfoMap(int index, PacketInfo Info, BOOLEAN Deleted);
void ShowRawData(HWND hDlg, int Index);
void ShowAnalysisData(HWND hDlg, int Index);
void StartScan(HWND hDlg);                     //����ARPɨ��
void Attack(HWND Hdlg);
extern int CurrentNum;
