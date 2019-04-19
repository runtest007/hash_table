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

extern hash_t *g_ClueHashTable;		//哈希表指针
extern int g_BucketCnt;				//桶个数
extern int g_BucketDepth;			//通深度，为0则表示桶深无限制
extern unsigned int g_ListTimeOut;	//超时时间
extern unsigned int g_inedex;			//插入数据的索引

extern unsigned int HashKeyFunc(void *keys);
extern int HashCompare(void *key, void *nodeKey);
extern void HashDestroyFunc(void * node);
extern long long HashNodeSizeCntFunc(void * node);

#endif



