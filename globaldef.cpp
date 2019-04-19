#include "globaldef.h"

hash_t *g_ClueHashTable = NULL;
int g_BucketCnt = 0;         
int g_BucketDepth = 0;	
unsigned int g_ListTimeOut = 300;
unsigned int g_inedex = 1;

unsigned int HashKeyFunc(void *keys)
{
	Packet_Info_T *key = (Packet_Info_T *)keys;
	unsigned int	nTmpDIp = key->nDIp;
	unsigned int	nTmpDPort = key->nDPort;
	unsigned int    nTmpIndex = key->nIndex;

	unsigned int hash_key =  ( (nTmpDPort & 0xff) << 8 ) | ( (nTmpIndex & 0xff) << 16 ) | ( (nTmpIndex & 0xff) << 24 );
	return hash_key;
}

int HashCompare(void *key, void *nodeKey)
{
	if(key == NULL || nodeKey == NULL)
	{
		return 0;
	}

	Packet_Info_T *newKey = (Packet_Info_T *)key;
	Packet_Info_T *oldKey = (Packet_Info_T *)nodeKey;

	if( newKey->nSIp == oldKey->nSIp 
		&&newKey->nSPort == oldKey->nSPort
		&&newKey->nDIp == oldKey->nDIp
		&&newKey->nDPort == oldKey->nDPort
		&&newKey->nIndex == oldKey->nIndex)
	{
		return 1;
	}
	
	return 0;
}


extern void HashDestroyFunc(void * node)
{
	if(node == NULL)
	{
		return;
	}

	bucket_node_ts *delNode = (bucket_node_ts*)node;
	
	if(delNode->data != NULL)
	{
		char *data = (char *)(delNode->data);
		delete data;
		data = NULL;
	}

	if(delNode->key != NULL)
	{
		Packet_Info_T *key = (Packet_Info_T *)delNode->key;
		delete key;
		key = NULL;
	}

	return;
}

long long HashNodeSizeCntFunc(void * node)
{
	if( NULL == node)
	{
		return 0;
	}
	bucket_node_ts *Node = (bucket_node_ts*)node;
	long long totalsize = 0;
	totalsize += sizeof(bucket_node_ts);
	totalsize += sizeof(Packet_Info_T);
	totalsize += sizeof(char);

	return totalsize;
}
