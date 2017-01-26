#pragma once
#include<Windows.h>
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
typedef struct _TCPPacket //20���ֽ�
{
	USHORT sourcePort; //16λԴ�˿ں�
	USHORT destinationPort;//16λĿ�Ķ˿ں�
	ULONG sequenceNumber; //32λ���к�
	ULONG acknowledgeNumber;//32λȷ�Ϻ�
	UCHAR dataoffset; //4λ�ײ�����/4λ������
	UCHAR flags; //6λ��־λ
	USHORT windows; //16λ���ڴ�С
	USHORT checksum; //16λУ���
	USHORT urgentPointer; //16λ��������ƫ����
}TCPPacket, *PTCPPacket;
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