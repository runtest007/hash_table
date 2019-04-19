#ifndef _HASH_H_
#define _HASH_H_

#include <time.h>
#include "list.h"

//******************************************************
//** 函数指针定义格式
//******************************************************
//	散列函数的函数指针定义
typedef unsigned int ( *hash_key_func_t ) (void *key);
//	删除哈息表节点中缓存数据的函数指针定义
typedef void ( *hash_destroy_func_t ) (void *data);
//	哈希键值比较函数指针定义
typedef int ( *hash_compare_t ) (void *key, void *nodeKey);
//	哈希节点占用内存大小计算函数指针定义
typedef long long ( *hash_nodesize_cnt_t ) (void *node);

//******************************************************
//** 节点定义
//******************************************************
//	桶头节点定义
typedef struct bucket_heads 
{
	int node_cnt;         // 桶下节点个数
	long long total_size;		  //节点占用内存大小(包括指向的内存大小)
	list_node_t head;     // 哈希链头

}bucket_head_ts;
//	桶节点
typedef struct bucket_nodes
{
	void *key;
	void *data;           // 保存数据
	time_t optTime;       // 加入时间
	list_node_t node;     // 哈希链节点
	list_node_t timeNode; // 时间链节点

}bucket_node_ts;
//	哈息表节点
typedef struct hash
{
	list_node_t timeHead;                         // 时间链头
	bucket_head_ts *bucket;                       // 桶
	int bucket_cnt;                               // 桶的个数
	int bucket_depth;							  //桶深
	hash_key_func_t key_func ;                    // 散列函数
	hash_compare_t compare_key ;                  // 键值比较函数
	hash_destroy_func_t destroy_node_func;        // 节点销毁函数
	hash_nodesize_cnt_t nodesize_cnt_func;		  //哈希节点占用内存大小计算函数

}hash_t;
//*************************************************************************
//** 功能：初始化哈希表
//** 参数：
//** bucket_cnt     ：桶的个数
//** bucket_depth	：桶的深度，设置为0表示不做限制
//** hash_key_func_t：哈希函数指针，设置NULL为默认函数
//** hash_compare_t ：键值比较函数指针，设置NULL为默认函数
//** hash_destroy_func_t：释放节点中data数据的函数指针，设置NULL为默认函数
//** hash_nodesize_cnt_t：哈希节点占用内存大小计算函数指针，设置NULL为默认函数
//**************************************************************************
hash_t *hash_init (	int	bucket_cnt, 
						hash_key_func_t     key_func, 
						hash_compare_t      compare_func,
						hash_destroy_func_t destroy_node_func,
						hash_nodesize_cnt_t nodesize_cnt_func = NULL,
						int	bucket_depth = 0);
//*************************************************************************
//** 功能：添加哈希节点
//** 参数：
//** table：哈希表指针
//** key  ：哈希key指针
//** data ：节点缓存数据指针
//*************************************************************************					
int hash_add ( hash_t *table, void *key, void *data ) ;
//*************************************************************************
//** 功能：查找哈希节点data（需要key和ID同时匹配）
//** 参数：
//** table：哈希表指针
//** key  ：查找节点key指针
//*************************************************************************
void *hash_lookup ( hash_t *table, void *key ) ;
//*************************************************************************
//** 功能：删除哈希节点（需要key和ID同时匹配）
//** 参数：
//** table：哈希表指针
//** key  ：需要删除的节点key指针
//*************************************************************************
int hash_remove ( hash_t *table, void *key ) ;
//*************************************************************************
//** 功能：更新节点时间（需要key和ID同时匹配）
//** 参数：
//** table ：哈希表指针
//** key   ：需要更新时间的节点key指针
//*************************************************************************
void hash_updateTime(hash_t *table, void *key );
//*************************************************************************
//** 功能：更新哈希桶统计信息
//** 参数：
//** table  ：哈希表指针
//** key：节点key指针
//**addcnt：增加的节点占用内存总大小
//*************************************************************************
void hash_updatebucketcnt(hash_t *table, void *key, long long addcnt);

//*************************************************************************
//** 功能：检查超时
//** 参数：
//** table  ：哈希表指针
//** seconds：超时时间，单位秒s
//*************************************************************************
void hash_checkTimeOut(hash_t *table, unsigned int seconds);

typedef void (*hashbrowse_callback) (void * key, void * data);
//*************************************************************************
//** 功能：遍历整个哈希表，并调用回调函数
//** 参数：
//** table：哈希表指针
//*************************************************************************
void hash_browse ( hash_t *table, hashbrowse_callback cb ) ;
//*************************************************************************
//** 功能：释放整个哈希表资源
//** 参数：
//** table：哈希表指针
//*************************************************************************
void free_hash ( hash_t *table ) ;



#endif
