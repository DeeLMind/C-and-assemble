#pragma once
#pragma warning(disable:4201)
#pragma warning(disable:4100)
#define NDIS_WDM
#define NDIS630
#include<wdm.h>
#include<ndis.h>


/*#define RECV_POOL_MAX 300*/
#define PACKET_DATA_MAX 1518
#define FILTER_MAJOR_NDIS_VERSION   NDIS_FILTER_MAJOR_VERSION
#define FILTER_MINOR_NDIS_VERSION   NDIS_FILTER_MINOR_VERSION
#define NETCFGGUID L"{5cbf81bd-5055-47cd-9055-a76b2b4e3697}"
#define SERVICENAME L"ZlzNdisLwf"
#define DEVICE_NAME L"\\Device\\Zlz Ndis6.30 Filter Kernel-Mode Device"
#define SYM_NAME L"\\??\\Zlz Ndis6.30 Filter SymbolLink"
#define PATH_MAX 255

#define IOCTL_SHOWADAPTER (ULONG)CTL_CODE(FILE_DEVICE_UNKNOWN,0x911,METHOD_BUFFERED,FILE_WRITE_DATA|FILE_READ_DATA) 
#define IOCTL_GETRAWDATA (ULONG)CTL_CODE(FILE_DEVICE_UNKNOWN,0x912,METHOD_BUFFERED,FILE_WRITE_DATA|FILE_READ_DATA) 

#define PACKET_TYPE_ADAPTERINFO 1
#define PACKET_TYPE_NETPACKET 2

PDRIVER_DISPATCH devcon;
typedef struct _AdapterInfo
{
	WCHAR DevPathName[PATH_MAX];
	WCHAR DevName[PATH_MAX];
	UCHAR MacAddress[32];
}AdapterInfo, *PAdapterInfo;
typedef struct _IO_Packet
{
	int Type;
	union 
	{
		struct 
		{
			int Num;
			AdapterInfo AdapterInfo[20];
		}ShowAdapter;
		struct
		{
			int Size;
			UCHAR Buffer[2000];         //MTU<=1500 
			BOOLEAN IsSendPacket;
			BOOLEAN Isthelast;
		}Net_Packet_Output;
		struct 
		{
			int ContextNum;
		}Net_Packet_InPut;
		unsigned u;
	}Packet;
}IO_Packet,*PIO_Packet;
typedef struct _S_PACKET
{
	LIST_ENTRY PacketList;
	int size;
	int MdlNumber;
	int MdlHasCopied;
	BOOLEAN IsSendPacket;
	PNET_BUFFER_LIST buffer;
	PMDL *mdllist;
}S_PACKET, *PS_PACKET;
typedef struct _DEVINCE_INFO
{
	NDIS_STRING DevPathName;                    //�豸·����
	NDIS_STRING DevName;                        //�豸����
	UCHAR MacAddress[32];                       //Mac��ַ
}DEVICE_INFO,*PDEVINCE_INFO;
typedef struct _FILTER_CONTEXT
{
	char magic[8];
	DEVICE_INFO DevInfo;
	NDIS_HANDLE FilterHandle;                   //�����豸������
	NDIS_HANDLE NetBufferPool;                  //���ؾ��
	BOOLEAN IsRunning;                          //�ù������Ƿ���������
	BOOLEAN IsFiltering;                        //�ù������Ƿ�����ݰ����й���
	int FliterIndex;                            //���˾����GLOBAL.context�е�������
	int CurrentRecvNum;                         //��ǰ����������ڵİ���
	LIST_ENTRY PacketRecvList;                  //��������
	KSPIN_LOCK NetBufferListLock;               //����ͬ��������
}FILTER_CONTEXT, *PFILTER_CONTEXT;
typedef struct _GLOBAL
{
	NDIS_HANDLE DriverHandle;          //ע������豸���õľ��
	int contextnum;                    //���豸��
	PFILTER_CONTEXT context[20];       //���豸������
	PDEVICE_OBJECT FilterDev;          //R3 R0 I/O�����豸
	UNICODE_STRING symname;            //���ӷ�����
	int RecvPoolMax;                   //��������С
}GLOBAL, *PGLOBAL;
typedef struct _MAC
{
	UCHAR dst[6];
	UCHAR sou[6];
	USHORT type;
}MAC,*PMAC;
GLOBAL Global;