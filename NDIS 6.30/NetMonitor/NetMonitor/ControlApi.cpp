#include"ControlApi.h"
#include<IPHlpApi.h>
#pragma warning(disable:4996)
void InitListInfo(PacketInfo Info, ListViewInfo *ListViewInfo)
{
	if (Info.Type == INFO_IPv6 || Info.Type == INFO_UNKNOWN)
	{
		if (Info.Type == INFO_UNKNOWN)
		{
			sprintf_s(ListViewInfo->Information, "Unknown protocol:%02x%02x\n", Info.RawPacket[20], Info.RawPacket[21]);
		}
		else
		{
			sprintf_s(ListViewInfo->ProtocolType, "IPv6\n");
		}
	}
	if (Info.Type == INFO_ARP)
	{
		UCHAR* saddr = Info.protocol.Arp.saddr;
		UCHAR* daddr = Info.protocol.Arp.daddr;
		if ((Info.Mac.dst[0] == 0xff) || (Info.Mac.dst[1] == 0xff))
		{
			sprintf_s(ListViewInfo->SourceIp, "%03d.%03d.%03d.%03d", saddr[0], saddr[1], saddr[2], saddr[3]);
			sprintf_s(ListViewInfo->DestIp, "BOARDCAST");
			sprintf_s(ListViewInfo->DataLength, "%4d", Info.Size);
			sprintf_s(ListViewInfo->ProtocolType, "%s", pro[Info.Type]);
		}
		else
		{
			sprintf_s(ListViewInfo->SourceIp, "%03d.%03d.%03d.%03d", saddr[0], saddr[1], saddr[2], saddr[3]);
			sprintf_s(ListViewInfo->DestIp, "%03d.%03d.%03d.%03d", daddr[0], daddr[1], daddr[2], daddr[3]);
			sprintf_s(ListViewInfo->DataLength, "%4d", Info.Size);
			sprintf_s(ListViewInfo->ProtocolType, "%s", pro[Info.Type]);
		}
		if (Tranverse16(Info.protocol.Arp.opcode) == ARP_REQUEST)
		{
			sprintf_s(ListViewInfo->Information, "who has ip %03d.%03d.%03d.%03d? tell %03d.%03d.%03d.%03d\n", Info.protocol.Arp.daddr[0],
				Info.protocol.Arp.daddr[1], Info.protocol.Arp.daddr[2], Info.protocol.Arp.daddr[3], Info.protocol.Arp.saddr[0],
				Info.protocol.Arp.saddr[1], Info.protocol.Arp.saddr[2], Info.protocol.Arp.saddr[3]);
		}
		else if (Tranverse16(Info.protocol.Arp.opcode) == ARP_REPLY)
		{
			sprintf_s(ListViewInfo->Information, "%03d.%03d.%03d.%03d is in mac %02x-%02x-%02x-%02x-%02x-%02x\n", Info.protocol.Arp.saddr[0], Info.protocol.Arp.saddr[1],
				Info.protocol.Arp.saddr[2], Info.protocol.Arp.saddr[3], Info.protocol.Arp.smac[0], Info.protocol.Arp.smac[1],
				Info.protocol.Arp.smac[2], Info.protocol.Arp.smac[3], Info.protocol.Arp.smac[4], Info.protocol.Arp.smac[5]);
		}
	}
	else
	{
		UCHAR* saddr = Info.protocol.Ip.ipSource;
		UCHAR* daddr = Info.protocol.Ip.ipDestination;
		sprintf_s(ListViewInfo->SourceIp, "%03d.%03d.%03d.%03d", saddr[0], saddr[1], saddr[2], saddr[3]);
		sprintf_s(ListViewInfo->DestIp, "%03d.%03d.%03d.%03d", daddr[0], daddr[1], daddr[2], daddr[3]);
		sprintf_s(ListViewInfo->DataLength, "%4d", Info.Size);
		sprintf_s(ListViewInfo->ProtocolType, "%s", pro[Info.Type]);
		if (Info.Type == INFO_TCP)
		{
			sprintf_s(ListViewInfo->Information, "window:%5d port:%d->%d ack:%d syn:%d fin:%d dataoffset:%d\n", Tranverse16(Info.protocol1.Tcp.windows), Tranverse16(Info.protocol1.Tcp.sourcePort), Tranverse16(Info.protocol1.Tcp.destinationPort), TCP_TEST_ACK(Info.protocol1.Tcp.flagsOffset),
				TCP_TEST_SYN(Info.protocol1.Tcp.flagsOffset), TCP_TEST_FIN(Info.protocol1.Tcp.flagsOffset), (TCP_GETDATAOFFSET(Info.protocol1.Tcp.flagsOffset)) * 4);
		}
		else if (Info.Type == INFO_ICMP)
		{
			sprintf_s(ListViewInfo->Information, "type:%02d code:%02d checksum:%d\n", Info.protocol1.Icmp.icmp_type, Info.protocol1.Icmp.icmp_code, Info.protocol1.Icmp.icmp_checksum);
		}
		else if (Info.Type == INFO_HTTP)
		{
			/*char http[60] = { 0 };
			int len = sizeof(MAC) + (Info.protocol.Ip.iphVerLen & 0x0f) * 4 + (TCP_GETDATAOFFSET(Info.protocol1.Tcp.flagsOffset) * 4)+1;
			memcpy(http, Info.RawPacket + len, sizeof(http)-1);
			for (int i = 0; i < 60; i++)
			{
			printf("%c", http[i]);
			}*/
			sprintf_s(ListViewInfo->Information, "");
		}
		else if (Info.Type == INFO_UDP)
		{
			sprintf_s(ListViewInfo->Information, "port:%d->%d\n", Tranverse16(Info.protocol1.Udp.sourcePort), Tranverse16(Info.protocol1.Udp.destinationPort));
		}
		else if (Info.Type == INFO_SSDPv4)
		{
			int len = sizeof(MAC) + (Info.protocol.Ip.iphVerLen & 0x0f) * 4 + sizeof(UDPPacket);
			/*for (int i = len; i < len + 30; i++)
			{
			if (Info.RawPacket[i] == '\n')
			{
			printf("\t");
			continue;
			}
			printf("%c", Info.RawPacket[i]);
			}
			printf("\n");*/
			memcpy(ListViewInfo->Information, &Info.RawPacket[len], 30);
		}
		else
		{
			sprintf_s(ListViewInfo->Information, "");
		}
	}
	/*SYSTEMTIME Systime= { 0 };
	GetSystemTime(&Systime);
	sprintf_s(timeorindex, "%d:%d:%d", Systime.wMinute, Systime.wSecond, Systime.wMilliseconds);*/
	sprintf_s(ListViewInfo->TimeOrIndex, "%d", CurrentNum);
}
VOID CALLBACK GetRawPacket(HWND hwnd, UINT msg, UINT_PTR timeid, DWORD systemtime)
{
	IO_Packet Output = { 0 };
	PacketInfo Info = { 0 };
	ListViewInfo ListViewInfo;
	if (Net_GetRawPacket(FilterHandle, &Output, StartIndex))
	{
		AnalysePacket(&Output, &Info);
	}
	else
	{
		return;
	}
	InitListInfo(Info, &ListViewInfo);
	ChangeListInfoMap(CurrentNum, Info, FALSE);
	AddListView(&ListViewInfo);
}
void StartFilter(HWND hDlg)
{
	int index = ListView_GetSelectedCount(GetDlgItem(hDlg, IDC_CARDLIST));
	StartIndex = index - 1;
	SetTimer(NULL, 1, 100, GetRawPacket);
	Net_StartFilter(FilterHandle, &StartIndex);
}
void StopFilter(HWND hDlg)
{
	int index = ListView_GetSelectedCount(GetDlgItem(hDlg, IDC_CARDLIST));
	if (index - 1 == StartIndex)
	{
		KillTimer(NULL, 1);
	}
	Net_StopFilter(FilterHandle, &StartIndex);
}
void FindCard(HWND hDlg)
{
	LVCOLUMN lvColumn;
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 150;
	HWND hList = GetDlgItem(hDlg, IDC_CARDLIST);
	SetWindowLong(hList, GWL_STYLE,
		WS_TABSTOP | WS_CHILD | WS_BORDER | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS);
	TCHAR       szString[4][20] = { TEXT("NetCardName"),
		TEXT("Mac"),
		TEXT("DevPathName"),
		TEXT("IsFiltering") };
	for (int i = 0; i < 4; i++)
	{
		lvColumn.pszText = szString[i];
		if (i == 2)
		{
			lvColumn.cx = 200;
		}
		ListView_InsertColumn(hList, i, &lvColumn);
	}

	FilterHandle = Net_OpenFilter();
	IO_Packet NetWorkAdapterInfo = { 0 };
	Net_ShowAdapter(FilterHandle, &NetWorkAdapterInfo);
	if (GetLastError() == ERROR_SUCCESS)
	{
		AdapterNum = NetWorkAdapterInfo.Packet.ShowAdapter.Num;
		for (int i = 0; i < AdapterNum; i++)
		{
			LVITEM lvi;
			ZeroMemory(&lvi, sizeof(lvi));
			lvi.mask = LVIF_TEXT;//|LVIF_IMAGE; 
			lvi.cchTextMax = 40;
			lvi.iSubItem = i;
			CHAR DevName[255] = { 0 };
			WideCharToMultiByte(CP_ACP, 0, NetWorkAdapterInfo.Packet.ShowAdapter.AdapterInfo->DevName,
				-1, (LPSTR)DevName, sizeof(DevName), NULL, NULL);
			lvi.pszText = DevName;
			lvi.iItem = 0;

			int now = ListView_InsertItem(hList, &lvi);//
			char buf[20] = { 0 };
			PUCHAR Mac = NetWorkAdapterInfo.Packet.ShowAdapter.AdapterInfo[i].MacAddress;
			sprintf_s(buf, "%02x-%02x-%02x-%02x-%02x-%02x\n", Mac[0], Mac[1], Mac[2], Mac[3], Mac[4], Mac[5]);
			ListView_SetItemText(hList, now, 1, buf);
			CHAR DevPathName[50] = { 0 };
			WideCharToMultiByte(CP_ACP, 0, NetWorkAdapterInfo.Packet.ShowAdapter.AdapterInfo->DevPathName,
				-1, (LPSTR)DevPathName, sizeof(DevPathName), NULL, NULL);
			ListView_SetItemText(hList, now, 2, DevPathName);
			ListView_SetItemText(hList, now, 3, NetWorkAdapterInfo.Packet.ShowAdapter.AdapterInfo->isFiltering ? "YES" : "NO");
		}
	}
	else
	{
		MessageBox(NULL, "Device Open Error!", "NetMonitor", MB_OK|MB_ICONERROR);
	}
}
void ShowOutput(LPNMLISTVIEW Mlv)
{
	int index = Mlv->iItem;
	char temp[20] = { 0 };
	LV_ITEM Item;
	Item.iItem = index;
	Item.iSubItem = 0;
	Item.mask = LVFIF_TEXT;
	Item.pszText = temp;
	Item.cchTextMax = sizeof(temp);
	ListView_GetItem(Mlv->hdr.hwndFrom,&Item);
	int trueindex = atoi(temp);
	auto iter = ListInfo.find(trueindex);
	if (iter != ListInfo.end())
	{
		char buf[255] = { 0 };
		char* point = buf;
		PacketInfo Info = ListInfo[trueindex];
		PUCHAR Mac = Info.Mac.sou;
		sprintf(point, "Source Mac Address:%02x-%02x-%02x-%02x-%02x-%02x\n", Mac[0], Mac[1], Mac[2], Mac[3],
			Mac[4], Mac[5]);
		point = buf + strlen(buf);
		Mac = Info.Mac.dst;
		sprintf(point, "Dest Mac Address:%02x-%02x-%02x-%02x-%02x-%02x\n", Mac[0], Mac[1], Mac[2], Mac[3],
			Mac[4], Mac[5]);
		SetWindowText(StaticWindow, buf);
	}
	else
	{
		SetWindowText(StaticWindow, "");
	}
}

void AddListView(PListViewInfo Info)
{
	LVITEM lvi;
	ZeroMemory(&lvi, sizeof(lvi));
	lvi.mask = LVIF_TEXT;//|LVIF_IMAGE; 
	lvi.cchTextMax = 40;
	lvi.iSubItem = 0;
	lvi.pszText = Info->TimeOrIndex;
	lvi.iItem = CurrentNum;

	int now = ListView_InsertItem(ListView, &lvi);//
	ListView_SetItemText(ListView, now, 1, Info->SourceIp);
	ListView_SetItemText(ListView, now, 2, Info->DestIp);
	ListView_SetItemText(ListView, now, 3, Info->DataLength);
	ListView_SetItemText(ListView, now, 4, Info->ProtocolType);
	ListView_SetItemText(ListView, now, 5, Info->Information);
	CurrentNum++;
}
void DeleteAllListInfo()
{
	ListView_DeleteAllItems(ListView);
	CurrentNum = 0;
}
void Refresh()
{
	ListView_DeleteAllItems(ListView);
	CurrentNum = 0;
	for (auto i = ListInfo.begin(); i != ListInfo.end(); i++)
	{
		PacketInfo Pi = i->second;
		ListViewInfo ListVInfo = { 0 };
		InitListInfo(Pi, &ListVInfo);
		AddListView(&ListVInfo);
	}
}
void ChangeListInfoMap(int index, PacketInfo Info,BOOLEAN Deleted)
{
	if (!Deleted)
	{
		ListInfo[index] = Info;
	}
	else
	{
		auto iter = ListInfo.find(index);
		if (iter != ListInfo.end())
		{
			ListInfo.erase(iter);
		}
	}
}
void ShowRawData(HWND hDlg,int Index)
{
	PacketInfo Info = ListInfo[Index];
	int Bufsize = Info.Size * 5;
	char *Buf = (char*)malloc(Bufsize);
	ZeroMemory(Buf, Bufsize);
	for (int i = 0; i < Info.Size; i++)
	{
		char temp[4] = { 0 };
		sprintf(temp, "%02X ", Info.RawPacket[i]);    //�ô�д��Сд�ᵼ�¶Բ�������
		strcat_s(Buf, Bufsize, temp);
	}
	HWND Edit = GetDlgItem(hDlg, IDC_RAWDATA);
	SetDlgItemText(hDlg, IDC_RAWDATA, Buf);
	free(Buf);
}

void PrintChar(int i,char *Buf,int Num,PacketInfo Info,int Bufsize)
{
	char temp[10] = { 0 };
	strcat_s(Buf, Bufsize, "\t\t");
	for (int j = 0; j < Num; j++)
	{
		if (Info.RawPacket[i + j] == 0 || Info.RawPacket[i + j] == '\n' || Info.RawPacket[i + j] == '\r')
		{
			sprintf(temp, ". ");
		}
		else
		{
			char a = Info.RawPacket[i + j];
			sprintf(temp, "%c", a);
		}
		strcat_s(Buf, Bufsize, temp);
	}
	strcat_s(Buf, Bufsize, "\r\n");
}
void ShowAnalysisData(HWND hDlg, int Index)
{
	char temp[10] = { 0 };
	PacketInfo Info = ListInfo[Index];
	int Bufsize = Info.Size * 6;
	char *Buf = (char*)malloc(Bufsize);
	ZeroMemory(Buf, Bufsize);
	for (int i = 0; i < Info.Size; i++)
	{
		if (i % 16 == 0 && i != 0)
		{
			PrintChar(i - 16, Buf, 16, Info, Bufsize);
		}
		else if (i % 8 == 0 && i != 0)
		{
			strcat_s(Buf, Bufsize, "\t\t");
		}
		sprintf(temp, "%02X ", Info.RawPacket[i]);      //�ô�д��Сд�ᵼ�¶Բ�������
		strcat_s(Buf, Bufsize, temp);
	}
	int al = Info.Size % 16;                              //���һ�еĶ���
	if (al)
	{
		for (int i = 0; i < 16 - al;i++)
		{
			sprintf(temp, "   ");
			strcat_s(Buf, Bufsize, temp);
		}
		strcat_s(Buf, Bufsize, "\t\t");
		PrintChar(Info.Size - al, Buf, al, Info, Bufsize);
	}
	HWND Edit = GetDlgItem(hDlg, IDC_RAWDATA);
	SetDlgItemText(hDlg, IDC_RAWDATA, Buf);
	free(Buf);
}

void StartScan(HWND hDlg)
{
	UCHAR IpStart[4] = { 0 };
	UCHAR IpEnd[4] = { 0 };
	UCHAR SourceAddr[4] = { 0 };
	UCHAR* Mac;
	
	HWND HIpStart = GetDlgItem(hDlg, IDC_IPSTART);
	HWND HIpEnd = GetDlgItem(hDlg, IDC_IPEND);
	HWND HSip = GetDlgItem(hDlg, IDC_LOCALIP);
	DWORD dwAddr;
	SendMessage(HIpStart, IPM_GETADDRESS, 0, (LPARAM)&dwAddr);
	IpStart[0] = FIRST_IPADDRESS(dwAddr);
	IpStart[1] = SECOND_IPADDRESS(dwAddr);
	IpStart[2] = THIRD_IPADDRESS(dwAddr);
	IpStart[3] = FOURTH_IPADDRESS(dwAddr);
	SendMessage(HIpEnd, IPM_GETADDRESS, 0, (LPARAM)&dwAddr);
	IpEnd[0] = FIRST_IPADDRESS(dwAddr);
	IpEnd[1] = SECOND_IPADDRESS(dwAddr);
	IpEnd[2] = THIRD_IPADDRESS(dwAddr);
	IpEnd[3] = FOURTH_IPADDRESS(dwAddr);
	SendMessage(HSip, IPM_GETADDRESS, 0, (LPARAM)&dwAddr);
	SourceAddr[0] = FIRST_IPADDRESS(dwAddr);
	SourceAddr[1] = SECOND_IPADDRESS(dwAddr);
	SourceAddr[2] = THIRD_IPADDRESS(dwAddr);
	SourceAddr[3] = FOURTH_IPADDRESS(dwAddr);
	if (FilterHandle != NULL)
	{
		IO_Packet Packet = { 0 };
		Net_ShowAdapter(FilterHandle, &Packet);
		Mac = Packet.Packet.ShowAdapter.AdapterInfo[StartIndex].MacAddress;
	}
	else
	{
		return;
	}
	RawPacket Packet = { 0 };
	for (int i = 0; i < 6; i++)
	{
		Packet.Osi.Mac.sou[i] = Mac[i];
		Packet.Osi.Mac.dst[i] = 0xff;
	}
	Packet.Osi.Mac.type = Tranverse16(PACKET_ARP);
	Packet.Osi.protocol.Arp.eth_type = Tranverse16(PACKET_IP);
	Packet.Osi.protocol.Arp.hrd = Tranverse16(1);
	Packet.Osi.protocol.Arp.maclen = 6;
	Packet.Osi.protocol.Arp.iplen = 4;
	Packet.Osi.protocol.Arp.opcode = Tranverse16(ARP_REQUEST);
	RtlCopyMemory(Packet.Osi.protocol.Arp.smac, Mac, sizeof(Packet.Osi.Mac.sou));
	RtlCopyMemory(Packet.Osi.protocol.Arp.saddr, SourceAddr, sizeof(SourceAddr));
	/*UCHAR daddr[4] = { 192,168,1,1 };        //��Ҫ��ȡMAC��ַ��IP
	RtlCopyMemory(Packet.Osi.protocol.Arp.daddr, daddr, sizeof(daddr));*/
	for (int a = IpStart[0]; a <= IpEnd[0]; a++)
	{
		for (int b = IpStart[1];  b<= IpEnd[1]; b++)
		{
			for (int c = IpStart[2]; c <= IpEnd[2]; c++)
			{
				for (int d = IpStart[3]; d <= IpEnd[3]; d++)
				{
					UCHAR DestAddr[4] = { a,b,c,d };
					RtlCopyMemory(Packet.Osi.protocol.Arp.daddr, DestAddr, sizeof(DestAddr));
					Net_SendRawPacket(FilterHandle, &Packet, ARPPACKETLENGTH, StartIndex);
				}
			}
		}
	}
	/*for (int i = 0; i < 255; i++)
	{
		UCHAR daddr[4] = { 192,168,1,i };        //��Ҫ��ȡMAC��ַ��IP
		RtlCopyMemory(Packet.Osi.protocol.Arp.daddr, daddr, sizeof(daddr));
		Net_SendRawPacket(FilterHandle, &Packet, 60, 0);
	}*/
}

void Attack(HWND Hdlg)
{

}