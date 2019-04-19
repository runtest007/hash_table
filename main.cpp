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
		cout << "节点的key为空，异常"<<endl;
		return;
	}

	Packet_Info_T *node_key = (Packet_Info_T *)key;
	printf("节点内容，SrcIP：%8u, SrcPort：%8u, DstIP: %8u, DstPort: %8u， Index：%8u.\n", node_key->nSIp, node_key->nSPort, node_key->nDIp, node_key->nDPort, node_key->nIndex);
	return;
}

bool init()
{
	int nBucketCnt = -1;
	int nBucketDepth = -1;
	unsigned int nTimeOut = 0;

	cout<<"请输入哈希表桶数:";
	cin>>nBucketCnt;
	cout<<"请输入哈希表桶深(输入0则表示桶深无限制):";
	cin>>nBucketDepth;
	cout<<"请输入超时时间(单位s):";
	cin>>nTimeOut;

	cout<<"输入的桶数为:"<<nBucketCnt<<", 桶深为:"<<nBucketDepth<<", 超时时间为:"<<nTimeOut<<"."<<endl;
	if(nBucketCnt <= 0 || nBucketDepth < 0 || nTimeOut == 0)
	{
		cout << "输入的桶数应该大于0, 输入的桶深应大于等于0, 超时时间应大于0 ."<<endl;
		return false;
	}
	else
	{
		g_BucketCnt = nBucketCnt;
		g_BucketDepth = nBucketDepth;
		g_ListTimeOut = nTimeOut;
	}

	cout<<"开始初始化哈希表"<<endl;
	g_ClueHashTable = hash_init(g_BucketCnt, HashKeyFunc, HashCompare, HashDestroyFunc, HashNodeSizeCntFunc, g_BucketDepth);
	if(g_ClueHashTable == NULL)
	{
		cout <<"哈希表初始化失败,程序退出."<<endl;
		return false;
	}
	else
	{
		cout<<"哈希表初始化成功."<<endl;
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

	printf("桶个数：%d，桶内存：%lld Bytes，桶节点个数：%u, 桶节点内存：%lld Bytes, 桶和桶节点总内存：%lld Bytes.\n", 
			g_ClueHashTable->bucket_cnt, nTotalBucketSizeCnt,  nTotalNodeCnt, nTotalNodeSizeCnt, nTotalNodeSizeCnt+nTotalBucketSizeCnt);
	return;
}

void add_node_func()
{
	unsigned int need_insert_cnt = 0;
	unsigned int cur_insert_cnt = 1;
	unsigned int err_insert_cnt = 0;
	unsigned int dup_insert_cnt = 0;

	cout<<"批量插入节点，请输入需要插入的节点数."<<endl;
	cin>>need_insert_cnt;
	if(need_insert_cnt == 0)
	{
		cout<<"输入需要插入的节点个数应该大于0"<<endl;
		return;
	}

	cout<<"开始批量插入节点"<<endl;
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
			cout<<"需要插入的节点已经存在，不进行插入."<<endl;
			g_inedex++;
			dup_insert_cnt++;	
			//return;
			continue;
		}

		Packet_Info_T *key = new Packet_Info_T;
		if(key == NULL)
		{
			cout<<"申请key内存失败，插入节点失败."<<endl;
			err_insert_cnt++;
			goto err_process;
		}

		char *data = new char(0x01);
		if(data == NULL)
		{
			cout<<"申请data内存失败，插入节点失败."<<endl;
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
			cout << "hash_add函数调用失败, 插入节点失败" <<endl;
		}

		g_inedex++;
	}

err_process:

	if(cur_insert_cnt > need_insert_cnt)
	{
		cur_insert_cnt = need_insert_cnt;
	}
	int after_tm = time(NULL);
	printf("批量插入节点结束,用时%d s,  需要插入%u节点，实际插入了%u次，其中插入失败%u次, 因重复而未插入%u次.\n", after_tm-befoe_tm, need_insert_cnt, cur_insert_cnt, err_insert_cnt, dup_insert_cnt);
	return;
}

void lookup_node_func()
{
	Packet_Info_T key;
	bzero(&key, sizeof(Packet_Info_T));
	cout <<"请输入源ip、源port、目的ip、目的端口、索引号"<<endl;	
	cin>>key.nSIp>>key.nSPort>>key.nDIp>>key.nDPort>>key.nIndex;

	Packet_Info_T *node_key = (Packet_Info_T *)hash_lookup(g_ClueHashTable, (void *)&key);
	if(node_key)
	{
		printf("查找到节点，SrcIP：%8u, SrcPort：%8u, DstIP: %8u, DstPort: %8u， Index：%8u.\n", node_key->nSIp, node_key->nSPort, node_key->nDIp, node_key->nDPort, node_key->nIndex);
	}
	else
	{
		cout<<"未查找到节点，节点不存在."<<endl;
	}

	return;
}


void remove_node_func()
{
	Packet_Info_T key;
	bzero(&key, sizeof(Packet_Info_T));
	cout <<"请输入源ip、源port、目的ip、目的端口、索引号"<<endl;	
	cin>>key.nSIp>>key.nSPort>>key.nDIp>>key.nDPort>>key.nIndex;

	if(0 == hash_remove(g_ClueHashTable, &key))
	{
		cout<<"节点删除成功."<<endl;
	}
	else
	{
		cout<<"节点删除失败."<<endl;		
	}
	
}

void display_node_func()
{
	hash_browse(g_ClueHashTable, display_node);
	return;
}


int main()
{
	//初始化hash表
	if(!init())
	{
		return -1;
	}

	int out_flag = 0;

	while(out_flag == 0)
	{
		int nOpt;
		cout<<"请输入操作类型：1 插入， 2 查询， 3 删除， 4 遍历打印  5 内存统计 6 退出"<<endl;
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
			cout << "操作类型输入错误，请重新输入.";
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
