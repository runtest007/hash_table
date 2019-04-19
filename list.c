#include "list.h"

//1. 初始化链表（宏）

//2. 向链表添加一个节点（函数），为基础函数
void _list_add( list_node_t *prev, 
                list_node_t *next,
                list_node_t *new_node )
{
        next->prev = new_node;
        new_node->next = next;
        new_node->prev = prev;
        prev->next = new_node;
}

//3. 在头节点或添加一节点
void list_add_head( struct list_node *head,
                    struct list_node *new_node )
{
        _list_add( head, head->next, new_node );
}

//4. 在链表的尾部添加一个节点
void list_add_tail( struct list_node *head,
                    struct list_node *new_node )
{
        _list_add( head->prev, head, new_node );
}

//5. 删除一个节点（函数），使用删除节点的前驱和后继，为基础函数
void list_del( struct list_node *prev, 
               struct list_node *next )
{
        prev->next = next;
        next->prev = prev;
}

//6．删除一个节点（函数），使用删除节点
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
 
//7. 将一个链表中的节点移动到另一个链表中的头节点后
void list_move( struct list_node *note,
                struct list_node *head )
{
        list_del( note->prev, note->next ) ;
        list_add_head(head, note);
}

//8. 将一个链表中的节点移动到另一个链表中的尾部
void list_move_tail( struct list_node *note, 
                     struct list_node *head )
{
        list_del( note->prev, note->next ) ;
        list_add_tail(head, note);
}

//9. 判断一个链表是否为空
int list_empty( const struct list_node *head )
{
        return ( head == head->next );
}

//10． 将一个链表连接到另一个链表后， 为基础函数
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

//11. 将一个链表连接到另一个链表后
void list_splice( struct list_node *list,
                  struct list_node *head )
{
        if ( !list_empty( list ) )
        {
                __list_splice( list, head );
                INIT_LIST_HEAD( list );
        }
}

//栈操作

//1. 初始化栈（宏）

//2. 压栈
void stack_push ( stack_node_t *head,
                  stack_node_t *node )
{
        list_add_tail( head, node );
}

//3. 弹栈
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

//队列操作

//1. 初始化队列（宏）

//2. 加入队列(entry)
void list_enque( queue_node_t *head,
                 queue_node_t *node )
{
        list_add_head( head, node );
}

//3. 取出队列元素(depart)
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
