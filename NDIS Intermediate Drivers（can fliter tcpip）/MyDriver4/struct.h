typedef struct _ETHeader //��̫������֡ͷ���ṹ
{
	UCHAR dhost[6]; //Ŀ��MAC��ַ
	UCHAR shost[6]; //ԴMAC��ַ
	USHORT type; //�²�Э�����ͣ���IP(ETHERTYPE_IP)��ARP��ETHERTYPE_ARP)��
}ETHeader, *PETHeader;
KEVENT queryeve;
KEVENT seteve;
typedef struct SEND_PACKET_RSVD
{
	PNDIS_PACKET            pOriginalPacket;
} SEND_PACKET_RSVD, *PSEND_PACKET_RSVD;
typedef struct _NPROT_RECV_PACKET_RSVD
{
	PNDIS_PACKET            pOriginalPacket;
} RECV_PACKET_RSVD, *PRECV_PACKET_RSVD;
typedef struct _adapercontext
{
	LIST_ENTRY packrecvlist;
	NDIS_HANDLE sendpacketpool;
	NDIS_HANDLE recvpacketpool;
	NDIS_HANDLE recvbufferpool;
	KSPIN_LOCK lock;
	NDIS_STATUS status;
	int contextno;
	NDIS_STRING DeviceName;
	NDIS_EVENT ndiseve;               //��������
	KEVENT unbindeve;             //�����
	KEVENT recveve;                   //�������ݰ��¼�
	BOOLEAN isnotgetdescriptor;
	NDIS_HANDLE relateminiporthandle;
	UINT seletemediumundex;
}adapercontext, *padapercontext;
typedef struct _miniportcontext
{
	int contextno;
	NDIS_HANDLE Miniportadapterhandle;
	NDIS_HANDLE relateadapterhandle;
	padapercontext relateadaptercontext;
	NDIS_REQUEST queryinformationrequest;
	NDIS_REQUEST setinformationrequest;
}miniportcontext, *pminiportcontext;
typedef struct _GLOBAL
{
	NDIS_HANDLE driverhandle;
	NDIS_HANDLE protocolhandle;
	padapercontext adaptercontext[20];   //���֧��20���豸
	NDIS_HANDLE bindinghandle[20];
	int contextnum;
	pminiportcontext miniportcontext[20];
	int mininum;
}GLOBAL, *PGLOBAL;
GLOBAL global;
PGLOBAL pglobal = &global;
typedef struct _Information
{
	PULONG sebyteread;
	PULONG seBytesNeeded;
	PULONG quBytesWritten;
	PULONG quBytesNeeded;
}Information, *pInformation;
Information requestinfo;
NDIS_MEDIUM         mediumarray[4] =
{
	NdisMedium802_3,    // Ethernet
	NdisMedium802_5,    // Token-ring
	NdisMediumFddi,     // Fddi
	NdisMediumWan       // NDISWAN
	/*NdisMediumCoWan*/
};
enum PacketType
{
	ZLZ_PACKET_RECEIVE,
	ZLZ_PACKET_SEND,
};
#pragma pack(push,1)
typedef struct _IpPacket
{
	UCHAR iphVerLen; //�汾�ź�ͷ����(��ռ4λ)
	UCHAR ipTOS; //��������
	USHORT ipLength; //����ܳ��ȣ�������IP���ĳ���
	USHORT ipID; //�����ʶ��Ωһ��ʶ���͵�ÿһ�����ݱ�
	USHORT ipFlags; //��־
	UCHAR ipTTL; //����ʱ�䣬����TTL
	UCHAR ipProtocol; //Э�飬������TCP��UDP��ICMP��
	USHORT ipChecksum; //У���
	ULONG ipSource; //ԴIP��ַ
	ULONG ipDestination; //Ŀ��IP��ַ
}IpPacket, *pIpPacket;
#pragma pack(pop)