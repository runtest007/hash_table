#include "list.h"

//1. ��ʼ�������꣩

//2. ���������һ���ڵ㣨��������Ϊ��������
void _list_add( list_node_t *prev, 
                list_node_t *next,
                list_node_t *new_node )
{
        next->prev = new_node;
        new_node->next = next;
        new_node->prev = prev;
        prev->next = new_node;
}

//3. ��ͷ�ڵ�����һ�ڵ�
void list_add_head( struct list_node *head,
                    struct list_node *new_node )
{
        _list_add( head, head->next, new_node );
}

//4. �������β�����һ���ڵ�
void list_add_tail( struct list_node *head,
                    struct list_node *new_node )
{
        _list_add( head->prev, head, new_node );
}

//5. ɾ��һ���ڵ㣨��������ʹ��ɾ���ڵ��ǰ���ͺ�̣�Ϊ��������
void list_del( struct list_node *prev, 
               struct list_node *next )
{
        prev->next = next;
        next->prev = prev;
}

//6��ɾ��һ���ڵ㣨��������ʹ��ɾ���ڵ�
void list_del_node( struct list_node *entry )
{
        if( entry == entry->next )
        {
                return ;
        }
        list_del( entry->prev, entry->next );
        entry->prev = entry;
        entry->next = entry;
}
 
//7. ��һ�������еĽڵ��ƶ�����һ�������е�ͷ�ڵ��
void list_move( struct list_node *note,
                struct list_node *head )
{
        list_del( note->prev, note->next ) ;
        list_add_head(head, note);
}

//8. ��һ�������еĽڵ��ƶ�����һ�������е�β��
void list_move_tail( struct list_node *note, 
                     struct list_node *head )
{
        list_del( note->prev, note->next ) ;
        list_add_tail(head, note);
}

//9. �ж�һ�������Ƿ�Ϊ��
int list_empty( const struct list_node *head )
{
        return ( head == head->next );
}

//10�� ��һ���������ӵ���һ������� Ϊ��������
void __list_splice ( struct list_node *list,
                     struct list_node *head )
{
        struct list_node *first = list->next;
        struct list_node *last = list->prev;
        struct list_node *at = head->next;
        
        first->prev = head;
        head->next = first;

        last->next = at;
        at->prev = last;
}

//11. ��һ���������ӵ���һ�������
void list_splice( struct list_node *list,
                  struct list_node *head )
{
        if ( !list_empty( list ) )
        {
                __list_splice( list, head );
                INIT_LIST_HEAD( list );
        }
}

//ջ����

//1. ��ʼ��ջ���꣩

//2. ѹջ
void stack_push ( stack_node_t *head,
                  stack_node_t *node )
{
        list_add_tail( head, node );
}

//3. ��ջ
stack_node_t *stack_pop ( stack_node_t *head )
{
        stack_node_t *node;

        if ( !list_empty( head ) )
        {
                node = head->prev;
                list_del_node( head->prev );
                return node;
        }
        else
        {
                return NULL;
        }
}

//���в���

//1. ��ʼ�����У��꣩

//2. �������(entry)
void list_enque( queue_node_t *head,
                 queue_node_t *node )
{
        list_add_head( head, node );
}

//3. ȡ������Ԫ��(depart)
queue_node_t *list_deque( queue_node_t *head )
{
        queue_node_t *node;

        if ( !list_empty( head ) )
        {
                node = head->prev;
                list_del_node( head->prev ) ;
                return node ;
        }
        else
        {
                return NULL ;
        }
}
