#pragma once
#include<Windows.h>

//�õ��ĺ�
#define Tranverse16(X)   ((((UINT16)(X) & 0xff00) >> 8) |(((UINT16)(X) & 0x00ff) << 8))    //����USHORT���С��ת��
#define SET_INFO_TYPE(A,B) (A->Type=B)

//�����жϰ�����
#define PACKET_IP   0x0800
#define PACKET_IPv6 0X86DD
#define PACKET_ARP  0X0806
#define PACKET_RARP 0X0835
#define PACKET_TCP  0X6
#define PACKET_IGMP 0X02
#define PACKET_ICMP 0x01
#define PACKET_UDP  0X11

//���а������ݽṹ
typedef struct _MAC
{
	UCHAR dst[6];
	UCHAR sou[6];
	USHORT type;
}MAC, *PMAC;
typedef struct _IPPacket
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
}IPPacket, *pIPPacket;
#pragma pack(push,1)
typedef struct _TCPPacket //20���ֽ�
{
	USHORT sourcePort; //16λԴ�˿ں�
	USHORT destinationPort;//16λĿ�Ķ˿ں�
	ULONG sequenceNumber; //32λ���к�
	ULONG acknowledgeNumber;//32λȷ�Ϻ�
	USHORT dataoffset : 4; //4λ�ײ�����
	USHORT reserved : 6; //6λ������
	USHORT urg : 1;
	USHORT ack : 1;
	USHORT psh : 1;
	USHORT rst : 1;
	USHORT syn : 1;
	USHORT fin : 1; //6λ��־λ
	USHORT windows; //16λ���ڴ�С
	USHORT checksum; //16λУ���
	USHORT urgentPointer; //16λ��������ƫ����
}TCPPacket, *PTCPPacket;
#pragma pack(pop)
typedef struct _UDPPacket
{
	USHORT sourcePort; //Դ�˿ں�
	USHORT destinationPort;//Ŀ�Ķ˿ں�
	USHORT len; //�������
	USHORT checksum; //У���
}UDPPacket, *PUDPPacket;
typedef struct _ICMPPacket
{
	UCHAR icmp_type; //��Ϣ����
	UCHAR icmp_code; //����
	USHORT icmp_checksum; //У���
						  //�����ǻ���ͷ
	USHORT icmp_id; //����Ωһ��ʶ�������ID�ţ�ͨ������Ϊ����ID
	USHORT icmp_sequence; //���к�
	ULONG icmp_timestamp; //ʱ���
}ICMPPacket, *PICMPPacket;
typedef struct _IGMPPacket //8�ֽ�
{
	UCHAR hVerType; //�汾�ź�����(��4λ)
	UCHAR uReserved; //δ��
	USHORT uCheckSum; //У���
	ULONG dwGroupAddress;//32Ϊ���ַ(D��IP��ַ)
}IGMPPacket, *PIGMPPacket;
typedef struct _ARPPacket //28�ֽڵ�ARPͷ
{
	USHORT hrd; //Ӳ����ַ�ռ䣬��̫����ΪARPHRD_EHER
	USHORT eth_type; //��̫�����ͣ�ETHERTYPE_IP
	UCHAR maclen; //MAC��ַ�ĳ��ȣ�Ϊ6
	UCHAR iplen; //IP��ַ�ĳ��ȣ�Ϊ4
	USHORT opcode; //��������,ARPOP_REWUESTΪ����ARPOP_REPLYΪ��Ӧ
	UCHAR smac[6]; //ԴMAC��ַ
	UCHAR saddr[4]; //ԴIP��ַ
	UCHAR dmac[6]; //Ŀ��MAC��ַ
	UCHAR daddr[4]; //Ŀ��IP��ַ
}ARPPacket, *PARPPacket;
typedef struct _DNSPacket 
{
	USHORT id; //��ʶ��ͨ�����ͻ��˿��Խ�DNS��������Ӧ����ƥ�䣻
	USHORT flags; //��־��(��ѯ)0x0100 (Ӧ��)0x8180 ��Щ���ֶ���������
	USHORT questions; //������Ŀ
	USHORT answers; //��Դ��¼��Ŀ
	USHORT author; //��Ȩ��Դ��¼��Ŀ
	USHORT addition; //������Դ��¼��Ŀ
}DNSPacket,*PDNSPacket;