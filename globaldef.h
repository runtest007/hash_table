#ifndef  _GLOBALDEF_H_
#define  _GLOBADEFL_H_

#include "hash.h"

typedef struct Packet_Info
{
	unsigned int	nSIp;
	unsigned int	nSPort;
	unsigned int	nDIp;
	unsigned int	nDPort;
	unsigned int    nIndex;
} Packet_Info_T;

extern hash_t *g_ClueHashTable;		//��ϣ��ָ��
extern int g_BucketCnt;				//Ͱ����
extern int g_BucketDepth;			//ͨ��ȣ�Ϊ0���ʾͰ��������
extern unsigned int g_ListTimeOut;	//��ʱʱ��
extern unsigned int g_inedex;			//�������ݵ�����

extern unsigned int HashKeyFunc(void *keys);
extern int HashCompare(void *key, void *nodeKey);
extern void HashDestroyFunc(void * node);
extern long long HashNodeSizeCntFunc(void * node);

#endif



