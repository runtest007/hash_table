#ifndef _HASH_H_
#define _HASH_H_

#include <time.h>
#include "list.h"

//******************************************************
//** ����ָ�붨���ʽ
//******************************************************
//	ɢ�к����ĺ���ָ�붨��
typedef unsigned int ( *hash_key_func_t ) (void *key);
//	ɾ����Ϣ��ڵ��л������ݵĺ���ָ�붨��
typedef void ( *hash_destroy_func_t ) (void *data);
//	��ϣ��ֵ�ȽϺ���ָ�붨��
typedef int ( *hash_compare_t ) (void *key, void *nodeKey);
//	��ϣ�ڵ�ռ���ڴ��С���㺯��ָ�붨��
typedef long long ( *hash_nodesize_cnt_t ) (void *node);

//******************************************************
//** �ڵ㶨��
//******************************************************
//	Ͱͷ�ڵ㶨��
typedef struct bucket_heads 
{
	int node_cnt;         // Ͱ�½ڵ����
	long long total_size;		  //�ڵ�ռ���ڴ��С(����ָ����ڴ��С)
	list_node_t head;     // ��ϣ��ͷ

}bucket_head_ts;
//	Ͱ�ڵ�
typedef struct bucket_nodes
{
	void *key;
	void *data;           // ��������
	time_t optTime;       // ����ʱ��
	list_node_t node;     // ��ϣ���ڵ�
	list_node_t timeNode; // ʱ�����ڵ�

}bucket_node_ts;
//	��Ϣ��ڵ�
typedef struct hash
{
	list_node_t timeHead;                         // ʱ����ͷ
	bucket_head_ts *bucket;                       // Ͱ
	int bucket_cnt;                               // Ͱ�ĸ���
	int bucket_depth;							  //Ͱ��
	hash_key_func_t key_func ;                    // ɢ�к���
	hash_compare_t compare_key ;                  // ��ֵ�ȽϺ���
	hash_destroy_func_t destroy_node_func;        // �ڵ����ٺ���
	hash_nodesize_cnt_t nodesize_cnt_func;		  //��ϣ�ڵ�ռ���ڴ��С���㺯��

}hash_t;
//*************************************************************************
//** ���ܣ���ʼ����ϣ��
//** ������
//** bucket_cnt     ��Ͱ�ĸ���
//** bucket_depth	��Ͱ����ȣ�����Ϊ0��ʾ��������
//** hash_key_func_t����ϣ����ָ�룬����NULLΪĬ�Ϻ���
//** hash_compare_t ����ֵ�ȽϺ���ָ�룬����NULLΪĬ�Ϻ���
//** hash_destroy_func_t���ͷŽڵ���data���ݵĺ���ָ�룬����NULLΪĬ�Ϻ���
//** hash_nodesize_cnt_t����ϣ�ڵ�ռ���ڴ��С���㺯��ָ�룬����NULLΪĬ�Ϻ���
//**************************************************************************
hash_t *hash_init (	int	bucket_cnt, 
						hash_key_func_t     key_func, 
						hash_compare_t      compare_func,
						hash_destroy_func_t destroy_node_func,
						hash_nodesize_cnt_t nodesize_cnt_func = NULL,
						int	bucket_depth = 0);
//*************************************************************************
//** ���ܣ���ӹ�ϣ�ڵ�
//** ������
//** table����ϣ��ָ��
//** key  ����ϣkeyָ��
//** data ���ڵ㻺������ָ��
//*************************************************************************					
int hash_add ( hash_t *table, void *key, void *data ) ;
//*************************************************************************
//** ���ܣ����ҹ�ϣ�ڵ�data����Ҫkey��IDͬʱƥ�䣩
//** ������
//** table����ϣ��ָ��
//** key  �����ҽڵ�keyָ��
//*************************************************************************
void *hash_lookup ( hash_t *table, void *key ) ;
//*************************************************************************
//** ���ܣ�ɾ����ϣ�ڵ㣨��Ҫkey��IDͬʱƥ�䣩
//** ������
//** table����ϣ��ָ��
//** key  ����Ҫɾ���Ľڵ�keyָ��
//*************************************************************************
int hash_remove ( hash_t *table, void *key ) ;
//*************************************************************************
//** ���ܣ����½ڵ�ʱ�䣨��Ҫkey��IDͬʱƥ�䣩
//** ������
//** table ����ϣ��ָ��
//** key   ����Ҫ����ʱ��Ľڵ�keyָ��
//*************************************************************************
void hash_updateTime(hash_t *table, void *key );
//*************************************************************************
//** ���ܣ����¹�ϣͰͳ����Ϣ
//** ������
//** table  ����ϣ��ָ��
//** key���ڵ�keyָ��
//**addcnt�����ӵĽڵ�ռ���ڴ��ܴ�С
//*************************************************************************
void hash_updatebucketcnt(hash_t *table, void *key, long long addcnt);

//*************************************************************************
//** ���ܣ���鳬ʱ
//** ������
//** table  ����ϣ��ָ��
//** seconds����ʱʱ�䣬��λ��s
//*************************************************************************
void hash_checkTimeOut(hash_t *table, unsigned int seconds);

typedef void (*hashbrowse_callback) (void * key, void * data);
//*************************************************************************
//** ���ܣ�����������ϣ�������ûص�����
//** ������
//** table����ϣ��ָ��
//*************************************************************************
void hash_browse ( hash_t *table, hashbrowse_callback cb ) ;
//*************************************************************************
//** ���ܣ��ͷ�������ϣ����Դ
//** ������
//** table����ϣ��ָ��
//*************************************************************************
void free_hash ( hash_t *table ) ;



#endif
