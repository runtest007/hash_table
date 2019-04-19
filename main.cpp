#include <iostream>
#include <stdio.h>
#include <string.h>
#include "globaldef.h"
#include "hash.h"

using std::cin;
using std::cout;
using std::endl;

#define ADD_NODE		1
#define LOOKUP_NODE		2
#define REMOVE_NODE		3
#define DISPLAY_NODE	4
#define MEM_STATIC		5
#define LOGOUT			6

#define SRC_IP_BASE		100000000
#define SRC_PORT_BASE   200000000
#define DST_IP_BASE		300000000
#define DST_PORT_BASE   400000000

void display_node(void *key, void *data)
{
	if(key == NULL)
	{
		cout << "�ڵ��keyΪ�գ��쳣"<<endl;
		return;
	}

	Packet_Info_T *node_key = (Packet_Info_T *)key;
	printf("�ڵ����ݣ�SrcIP��%8u, SrcPort��%8u, DstIP: %8u, DstPort: %8u�� Index��%8u.\n", node_key->nSIp, node_key->nSPort, node_key->nDIp, node_key->nDPort, node_key->nIndex);
	return;
}

bool init()
{
	int nBucketCnt = -1;
	int nBucketDepth = -1;
	unsigned int nTimeOut = 0;

	cout<<"�������ϣ��Ͱ��:";
	cin>>nBucketCnt;
	cout<<"�������ϣ��Ͱ��(����0���ʾͰ��������):";
	cin>>nBucketDepth;
	cout<<"�����볬ʱʱ��(��λs):";
	cin>>nTimeOut;

	cout<<"�����Ͱ��Ϊ:"<<nBucketCnt<<", Ͱ��Ϊ:"<<nBucketDepth<<", ��ʱʱ��Ϊ:"<<nTimeOut<<"."<<endl;
	if(nBucketCnt <= 0 || nBucketDepth < 0 || nTimeOut == 0)
	{
		cout << "�����Ͱ��Ӧ�ô���0, �����Ͱ��Ӧ���ڵ���0, ��ʱʱ��Ӧ����0 ."<<endl;
		return false;
	}
	else
	{
		g_BucketCnt = nBucketCnt;
		g_BucketDepth = nBucketDepth;
		g_ListTimeOut = nTimeOut;
	}

	cout<<"��ʼ��ʼ����ϣ��"<<endl;
	g_ClueHashTable = hash_init(g_BucketCnt, HashKeyFunc, HashCompare, HashDestroyFunc, HashNodeSizeCntFunc, g_BucketDepth);
	if(g_ClueHashTable == NULL)
	{
		cout <<"��ϣ���ʼ��ʧ��,�����˳�."<<endl;
		return false;
	}
	else
	{
		cout<<"��ϣ���ʼ���ɹ�."<<endl;
	}

	return true;
}

void display_hashtable_mem_size()
{

	unsigned int nTotalNodeCnt = 0;
	long long nTotalNodeSizeCnt = 0;
	long long nTotalBucketSizeCnt = 0;

	for ( int i = 0; i < g_ClueHashTable->bucket_cnt; i++ )
	{
		nTotalNodeCnt += g_ClueHashTable->bucket[i].node_cnt ;
		nTotalNodeSizeCnt += g_ClueHashTable->bucket[i].total_size;
	}

	nTotalBucketSizeCnt = g_ClueHashTable->bucket_cnt * sizeof(bucket_head_ts);

	printf("Ͱ������%d��Ͱ�ڴ棺%lld Bytes��Ͱ�ڵ������%u, Ͱ�ڵ��ڴ棺%lld Bytes, Ͱ��Ͱ�ڵ����ڴ棺%lld Bytes.\n", 
			g_ClueHashTable->bucket_cnt, nTotalBucketSizeCnt,  nTotalNodeCnt, nTotalNodeSizeCnt, nTotalNodeSizeCnt+nTotalBucketSizeCnt);
	return;
}

void add_node_func()
{
	unsigned int need_insert_cnt = 0;
	unsigned int cur_insert_cnt = 1;
	unsigned int err_insert_cnt = 0;
	unsigned int dup_insert_cnt = 0;

	cout<<"��������ڵ㣬��������Ҫ����Ľڵ���."<<endl;
	cin>>need_insert_cnt;
	if(need_insert_cnt == 0)
	{
		cout<<"������Ҫ����Ľڵ����Ӧ�ô���0"<<endl;
		return;
	}

	cout<<"��ʼ��������ڵ�"<<endl;
	Packet_Info_T tmp_key;
	int befoe_tm = time(NULL);
	for(; cur_insert_cnt <= need_insert_cnt; cur_insert_cnt++)
	{
		bzero(&tmp_key, sizeof(Packet_Info_T));
		tmp_key.nSIp = SRC_IP_BASE + cur_insert_cnt%SRC_IP_BASE;
		tmp_key.nSPort = SRC_PORT_BASE + cur_insert_cnt%SRC_PORT_BASE;
		tmp_key.nDIp = DST_IP_BASE + cur_insert_cnt% DST_IP_BASE;
		tmp_key.nDPort= DST_PORT_BASE + cur_insert_cnt%DST_PORT_BASE;
		tmp_key.nIndex = g_inedex % 4000000000;

		if(NULL != hash_lookup(g_ClueHashTable, &tmp_key))
		{
			cout<<"��Ҫ����Ľڵ��Ѿ����ڣ������в���."<<endl;
			g_inedex++;
			dup_insert_cnt++;	
			//return;
			continue;
		}

		Packet_Info_T *key = new Packet_Info_T;
		if(key == NULL)
		{
			cout<<"����key�ڴ�ʧ�ܣ�����ڵ�ʧ��."<<endl;
			err_insert_cnt++;
			goto err_process;
		}

		char *data = new char(0x01);
		if(data == NULL)
		{
			cout<<"����data�ڴ�ʧ�ܣ�����ڵ�ʧ��."<<endl;
			if( NULL != key)
			{
				delete key;
				key = NULL;
			}
			err_insert_cnt++;
			goto err_process;
		}
	
		key->nSIp = tmp_key.nSIp;
		key->nSPort = tmp_key.nSPort;
		key->nDIp = tmp_key.nDIp;
		key->nDPort= tmp_key.nDPort;
		key->nIndex = tmp_key.nIndex;

		if(0 != hash_add(g_ClueHashTable, (void *)key, data))
		{
			cout << "hash_add��������ʧ��, ����ڵ�ʧ��" <<endl;
		}

		g_inedex++;
	}

err_process:

	if(cur_insert_cnt > need_insert_cnt)
	{
		cur_insert_cnt = need_insert_cnt;
	}
	int after_tm = time(NULL);
	printf("��������ڵ����,��ʱ%d s,  ��Ҫ����%u�ڵ㣬ʵ�ʲ�����%u�Σ����в���ʧ��%u��, ���ظ���δ����%u��.\n", after_tm-befoe_tm, need_insert_cnt, cur_insert_cnt, err_insert_cnt, dup_insert_cnt);
	return;
}

void lookup_node_func()
{
	Packet_Info_T key;
	bzero(&key, sizeof(Packet_Info_T));
	cout <<"������Դip��Դport��Ŀ��ip��Ŀ�Ķ˿ڡ�������"<<endl;	
	cin>>key.nSIp>>key.nSPort>>key.nDIp>>key.nDPort>>key.nIndex;

	Packet_Info_T *node_key = (Packet_Info_T *)hash_lookup(g_ClueHashTable, (void *)&key);
	if(node_key)
	{
		printf("���ҵ��ڵ㣬SrcIP��%8u, SrcPort��%8u, DstIP: %8u, DstPort: %8u�� Index��%8u.\n", node_key->nSIp, node_key->nSPort, node_key->nDIp, node_key->nDPort, node_key->nIndex);
	}
	else
	{
		cout<<"δ���ҵ��ڵ㣬�ڵ㲻����."<<endl;
	}

	return;
}


void remove_node_func()
{
	Packet_Info_T key;
	bzero(&key, sizeof(Packet_Info_T));
	cout <<"������Դip��Դport��Ŀ��ip��Ŀ�Ķ˿ڡ�������"<<endl;	
	cin>>key.nSIp>>key.nSPort>>key.nDIp>>key.nDPort>>key.nIndex;

	if(0 == hash_remove(g_ClueHashTable, &key))
	{
		cout<<"�ڵ�ɾ���ɹ�."<<endl;
	}
	else
	{
		cout<<"�ڵ�ɾ��ʧ��."<<endl;		
	}
	
}

void display_node_func()
{
	hash_browse(g_ClueHashTable, display_node);
	return;
}


int main()
{
	//��ʼ��hash��
	if(!init())
	{
		return -1;
	}

	int out_flag = 0;

	while(out_flag == 0)
	{
		int nOpt;
		cout<<"������������ͣ�1 ���룬 2 ��ѯ�� 3 ɾ���� 4 ������ӡ  5 �ڴ�ͳ�� 6 �˳�"<<endl;
		cin >> nOpt;

		switch (nOpt)
		{
		case ADD_NODE:
			add_node_func();
			break;

		case LOOKUP_NODE:
			lookup_node_func();
			break;

		case REMOVE_NODE:
			remove_node_func();
			break;

		case DISPLAY_NODE:
			display_node_func();
			break;

		case MEM_STATIC:
			display_hashtable_mem_size();
			break;

		case LOGOUT:
			out_flag = 1;
			break;

		default:
			cout << "�������������������������.";
			break;
		}

	}

	if( NULL != g_ClueHashTable)
	{
		free_hash(g_ClueHashTable);
		g_ClueHashTable = NULL;
	}

	return 0;

}
