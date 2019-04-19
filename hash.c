#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "hash.h"

#define safe_malloc malloc
#define safe_free free

static unsigned int hash_key_func_string ( void *pkey ) ;
static int hash_compare_func_string (void *key, void *nodeKey);

//默认散列函数，已字符串为KEY
static unsigned int hash_key_func_string ( void *pkey )
{
        char   *key = (char *) pkey;
        unsigned long h = 0, g;

        assert (key != 0);
        for (; *key; key++)
        {
                h = (h << 4) + tolower (*key);
                if ((g = h & 0xF0000000))
                        h ^= g >> 24;
                h &= ~g;
        }
        return h;
}

//默认比较函数
static int hash_compare_func_string (void *key, void *nodeKey)
{
	   if(key == NULL || nodeKey == NULL)
	   {
			return 0;
	   }
	   return strcmp ( ( char * )key, ( char * )nodeKey ) == 0 ? 1 : 0 ;
}

// 查找桶节点，功能辅助函数
void *FindNode ( hash_t *table, void *key )
{
        int err_num = 0 ;
        bucket_head_ts *lpbh = NULL ;
        bucket_node_ts *lpbn = NULL ;
        unsigned int sum = 0 ;
        void *data = NULL;
        list_node_t *lpnode = NULL ;
        list_node_t *lphead = NULL ;

        if ( table == NULL || key == NULL )
        {
            err_num = 1 ;
            goto hash_lookup_err ;
        }

        //根据散列函数，取桶的序列号
        sum = table->key_func ( key ) % table->bucket_cnt ;
        lpbh = &( table->bucket[ sum ] ) ;

        //在桶的链表中查找节点
        lphead = &( lpbh->head ) ;
        lpnode = lphead->next ;

        while ( lpnode != lphead )
        {
                lpbn = LIST_ENTRY ( lpnode, bucket_node_ts, node ) ;
                //根据比较KEY的值来确定是否找到节点
                if ( table->compare_key (key, lpbn->key) )
                {
                        data = lpbn ;
                        break ;
                }
                lpnode = lpnode->next ;
        }

hash_lookup_err : 
        switch ( err_num )
        {
        case 1 : 
                break ;
        }
        
        return data ;
}
//哈息表初始化函数
hash_t *hash_init (	int	bucket_cnt, 
						hash_key_func_t     key_func, 
						hash_compare_t      compare_func,
						hash_destroy_func_t destroy_node_func,
						hash_nodesize_cnt_t nodesize_cnt_func, 
						int	bucket_depth)
{
        int err_num = 0 ;        
        hash_t *lphash = NULL ;
        int i = 0 ;
        
        assert( bucket_cnt > 0 ) ;

        //初始化hash头
        lphash = ( hash_t * )safe_malloc( sizeof( hash_t ) ) ;
        if( lphash == NULL )
        {
                err_num = 1 ;
                goto hash_init_err ;
        }

        //初始化桶
        lphash->bucket_cnt = bucket_cnt ;
		lphash->bucket_depth = bucket_depth ;
        lphash->bucket = ( bucket_head_ts * )safe_malloc( sizeof( bucket_head_ts ) * bucket_cnt ) ;
		memset(lphash->bucket, 0 , sizeof( bucket_head_ts ) * bucket_cnt );//add by zxh 20151023 init hash_bucket_cnt
        if ( lphash->bucket == NULL )
        {
                err_num = 2 ;
                goto hash_init_err ;
        }
        
        //初始化时间链头节点
		INIT_LIST_HEAD ( &(lphash->timeHead) ) ; 
	
		//初始化每个桶下的链表头节点
        for ( i = 0; i < bucket_cnt; i++ )
        {
                INIT_LIST_HEAD ( &( lphash->bucket[i].head ) ) ;
        }
        
        //如果初始化散列函数为空，那么取默认散列函数
        if ( key_func == NULL ) 
        {
                lphash->key_func = hash_key_func_string ;
        }
        else 
        {
                lphash->key_func = key_func ;
        }
        //如果比较KEY的函数为空，那么取默认比较函数
        if ( compare_func == NULL )
        {
                lphash->compare_key = hash_compare_func_string ;
        }
        else 
        {
                lphash->compare_key = compare_func ;
        }
        lphash->destroy_node_func = destroy_node_func ;
		lphash->nodesize_cnt_func = nodesize_cnt_func ;

hash_init_err : 
        switch ( err_num )
        {
        case 2 : 
                safe_free ( lphash );
                lphash = NULL ;
        case 1 : 
                break ;
        }
        
        return lphash ;
}

//向hash表添加数据
int hash_add ( hash_t *table, void *key, void *data )
{
        int err_num = 0 ;
        bucket_node_ts *lpbn = NULL ;
        unsigned int sum = 0 ;
	
		if( table == NULL || key == NULL || data == NULL )
		{
			err_num = 1 ;
            goto hash_add_err ;
		}

        //创建一个桶节点
        lpbn = ( bucket_node_ts * )safe_malloc ( sizeof ( bucket_node_ts ) );
        if ( lpbn == NULL )
        {
                err_num = 2 ;
                goto hash_add_err ;
        }

		lpbn->optTime = time(NULL);
        lpbn->data = data;
        lpbn->key = key ;
        sum = table->key_func ( key ) % table->bucket_cnt;
		if(table->bucket_depth != 0 && table->bucket[sum].node_cnt > table->bucket_depth)
		{
			err_num = 3;
			goto hash_add_err ;
		}
        list_add_tail( &(table->bucket[sum].head), 
                       &(lpbn->node) );
		// 加入时间链
		list_add_tail( &table->timeHead,
						&(lpbn->timeNode) );
		if ( table->nodesize_cnt_func != NULL ) 
		{
			table->bucket[sum].total_size += table->nodesize_cnt_func(lpbn);
		}
        ++( table->bucket[sum].node_cnt ) ;
hash_add_err : 
        switch ( err_num ) 
        {
        case 1 : 
                break ;
        }

        return err_num;
}

//查找节点
void *hash_lookup ( hash_t *table, void *key )
{
		if( table == NULL || key == NULL )
		{
			return NULL;
		}
        bucket_node_ts *node = (bucket_node_ts*)FindNode(table, key);
		if(node)
		{
			/*为了测试，临时修改*/
			//return  node->data;
			return node->key; 
		}
		else
		{
			return NULL;
		}
}

//删除某节点
int hash_remove ( hash_t *table, void *key )
{
		if( table == NULL || key == NULL )
		{
			return 1;
		}
		unsigned int sum = table->key_func ( key ) % table->bucket_cnt ;
		bucket_node_ts *lpbn = (bucket_node_ts*)FindNode(table, key);
		if(lpbn)
		{
			if ( table->nodesize_cnt_func != NULL ) 
			{
				table->bucket[sum].total_size -= table->nodesize_cnt_func(lpbn);
			}
			if ( table->destroy_node_func != NULL ) 
            {
                 table->destroy_node_func( lpbn );
            }
            list_del_node ( &( lpbn->node ) ) ;
			// 摘取时间链
			list_del_node ( &( lpbn->timeNode));
            safe_free ( lpbn ) ;
            lpbn = NULL ;
			--( table->bucket[ sum ].node_cnt ) ;
			return  0;
		}
		else
		{
			return 1;
		}
}

void free_hash ( hash_t *table )
{
        int i ;
        bucket_head_ts *lpbh = NULL ;
        bucket_node_ts *lpbn = NULL ;
        list_node_t *lpnode = NULL ;
        list_node_t *lphead = NULL ;

		if( NULL == table )
		{
			return;
		}
        for ( i = 0; i < table->bucket_cnt; i++ )
        {
                lpbh = &( table->bucket[ i ] ) ;
                
                lphead = &( lpbh->head ) ;
                lpnode = lphead->next ;
                while ( lpnode != lphead )
                {
                        lpbn = LIST_ENTRY ( lpnode, bucket_node_ts, node ) ;

                        if ( table->destroy_node_func != NULL ) 
                        {
                                table->destroy_node_func( lpbn ) ;
                        }

                        lpnode = lpnode->next ;

                        list_del_node ( &( lpbn->node ) ) ;
                        safe_free ( lpbn ) ;
                        lpbn = NULL ;
                }
        }
        
        safe_free ( table->bucket );
        safe_free ( table );
}
// 超时检查函数
void hash_checkTimeOut( hash_t *table, unsigned int seconds)
{
	if( NULL == table )
	{
		return;
	}
	while( table->timeHead.next != &(table->timeHead) )
	{
		//取得桶节点
		//bucket_node_t *bnode = (bucket_node_t *)(table->timeHead.next+1)-1;
		list_node_t *lptNode = (list_node_t*)table->timeHead.next;
		bucket_node_ts *bnode = LIST_ENTRY ( lptNode, bucket_node_ts, timeNode ) ;
		if(bnode)
		{
			time_t optime = bnode->optTime;
			time_t timenow = time(NULL);
			if( (timenow - optime) > seconds )
			{
				unsigned int sum = table->key_func ( bnode->key ) % table->bucket_cnt ;
				if ( table->nodesize_cnt_func != NULL ) 
				{
					table->bucket[sum].total_size -= table->nodesize_cnt_func(bnode);
				}
				if ( table->destroy_node_func != NULL ) 
				{
					table->destroy_node_func( bnode ) ;
				}
				// 摘取哈希链
				list_del_node ( &(bnode->node) ) ;
				// 摘取时间链
				list_del_node ( &(bnode->timeNode) );
				//删除桶节点
				safe_free ( bnode) ;
				bnode = NULL ;
				--( table->bucket[ sum ].node_cnt ) ;
				continue;

			}
			else
			{
				return;
			}
		}
	}

}
// 更新时间节点
void hash_updateTime(hash_t *table, void *key)
{
	if( table == NULL || key == NULL )
	{
		return;
	}
	 bucket_node_ts *bnode = (bucket_node_ts*)FindNode(table, key);
	 if(bnode)
	 {
		bnode->optTime = time(NULL);
		list_del_node( &(bnode->timeNode) );
		list_add_tail( &(table->timeHead), &(bnode->timeNode) );
	}
}

// 更新桶统计信息
void hash_updatebucketcnt(hash_t *table, void *key, long long addcnt)
{
	if( table == NULL || key == NULL )
	{
		return;
	}
	unsigned int sum = table->key_func ( key ) % table->bucket_cnt ;
	if ( table->nodesize_cnt_func != NULL ) 
	{
		table->bucket[sum].total_size += addcnt;
	}
}

void hash_browse ( hash_t *table, hashbrowse_callback cb ) 
{
    bucket_head_ts *lpbh = NULL ;
    bucket_node_ts *lpbn = NULL ;
	int i;
    list_node_t *lpnode = NULL ;
    list_node_t *lphead = NULL ;

    if ( table == NULL)
    	return;

	for(i = 0 ; i < table->bucket_cnt ; i++)
	{
		 lpbh = &( table->bucket[ i ] ) ;
		 
		 //在桶的链表中查找节点
		 lphead = &( lpbh->head ) ;
		 lpnode = lphead->next ;
		 
		 while ( lpnode != lphead )
		 {
				lpbn = LIST_ENTRY ( lpnode, bucket_node_ts, node ) ;
				cb(lpbn->key,lpbn->data);
				lpnode = lpnode->next ;
		 }
	}

}

