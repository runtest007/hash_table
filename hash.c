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

//Ĭ��ɢ�к��������ַ���ΪKEY
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

//Ĭ�ϱȽϺ���
static int hash_compare_func_string (void *key, void *nodeKey)
{
	   if(key == NULL || nodeKey == NULL)
	   {
			return 0;
	   }
	   return strcmp ( ( char * )key, ( char * )nodeKey ) == 0 ? 1 : 0 ;
}

// ����Ͱ�ڵ㣬���ܸ�������
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

        //����ɢ�к�����ȡͰ�����к�
        sum = table->key_func ( key ) % table->bucket_cnt ;
        lpbh = &( table->bucket[ sum ] ) ;

        //��Ͱ�������в��ҽڵ�
        lphead = &( lpbh->head ) ;
        lpnode = lphead->next ;

        while ( lpnode != lphead )
        {
                lpbn = LIST_ENTRY ( lpnode, bucket_node_ts, node ) ;
                //���ݱȽ�KEY��ֵ��ȷ���Ƿ��ҵ��ڵ�
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
//��Ϣ���ʼ������
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

        //��ʼ��hashͷ
        lphash = ( hash_t * )safe_malloc( sizeof( hash_t ) ) ;
        if( lphash == NULL )
        {
                err_num = 1 ;
                goto hash_init_err ;
        }

        //��ʼ��Ͱ
        lphash->bucket_cnt = bucket_cnt ;
		lphash->bucket_depth = bucket_depth ;
        lphash->bucket = ( bucket_head_ts * )safe_malloc( sizeof( bucket_head_ts ) * bucket_cnt ) ;
		memset(lphash->bucket, 0 , sizeof( bucket_head_ts ) * bucket_cnt );//add by zxh 20151023 init hash_bucket_cnt
        if ( lphash->bucket == NULL )
        {
                err_num = 2 ;
                goto hash_init_err ;
        }
        
        //��ʼ��ʱ����ͷ�ڵ�
		INIT_LIST_HEAD ( &(lphash->timeHead) ) ; 
	
		//��ʼ��ÿ��Ͱ�µ�����ͷ�ڵ�
        for ( i = 0; i < bucket_cnt; i++ )
        {
                INIT_LIST_HEAD ( &( lphash->bucket[i].head ) ) ;
        }
        
        //�����ʼ��ɢ�к���Ϊ�գ���ôȡĬ��ɢ�к���
        if ( key_func == NULL ) 
        {
                lphash->key_func = hash_key_func_string ;
        }
        else 
        {
                lphash->key_func = key_func ;
        }
        //����Ƚ�KEY�ĺ���Ϊ�գ���ôȡĬ�ϱȽϺ���
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

//��hash���������
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

        //����һ��Ͱ�ڵ�
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
		// ����ʱ����
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

//���ҽڵ�
void *hash_lookup ( hash_t *table, void *key )
{
		if( table == NULL || key == NULL )
		{
			return NULL;
		}
        bucket_node_ts *node = (bucket_node_ts*)FindNode(table, key);
		if(node)
		{
			/*Ϊ�˲��ԣ���ʱ�޸�*/
			//return  node->data;
			return node->key; 
		}
		else
		{
			return NULL;
		}
}

//ɾ��ĳ�ڵ�
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
			// ժȡʱ����
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
// ��ʱ��麯��
void hash_checkTimeOut( hash_t *table, unsigned int seconds)
{
	if( NULL == table )
	{
		return;
	}
	while( table->timeHead.next != &(table->timeHead) )
	{
		//ȡ��Ͱ�ڵ�
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
				// ժȡ��ϣ��
				list_del_node ( &(bnode->node) ) ;
				// ժȡʱ����
				list_del_node ( &(bnode->timeNode) );
				//ɾ��Ͱ�ڵ�
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
// ����ʱ��ڵ�
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

// ����Ͱͳ����Ϣ
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
		 
		 //��Ͱ�������в��ҽڵ�
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

