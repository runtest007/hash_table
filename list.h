#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>

//链表操作

//链表的节点
typedef struct list_node
{
        struct list_node *prev;
        struct list_node *next;
}list_node_t;

//1. 初始化链表（宏）
#define INIT_LIST_HEAD(ptr) do { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

//zxj
#define INIT_LIST_NODE(ptr) do { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)


//2. 向链表添加一个节点（函数）
void _list_add( list_node_t *prev,
                list_node_t *next,
                list_node_t *new_node ) ;

//3. 在头节点或添加一节点
void list_add_head( list_node_t *head,
                    list_node_t *new_node ) ;

//4. 在链表的尾部添加一个节点
void list_add_tail( list_node_t *head,
                    list_node_t *new_node ) ;


//5. 删除一个节点（函数），使用删除节点的前驱和后继
void list_del( list_node_t *prev, 
               list_node_t *next );

//6．删除一个节点（函数），使用删除节点
void list_del_node( list_node_t *entry ) ;

//7. 将一个链表中的节点移动到另一个链表中的头节点后
void list_move( list_node_t *note,
                list_node_t *head );

//8. 将一个链表中的节点移动到另一个链表中的尾部
void list_move_tail( list_node_t *note, 
                     list_node_t *head ) ;

//9. 判断一个链表是否为空
int list_empty( const list_node_t *head ) ;

//10． 将一个链表连接到另一个链表后， 为基础函数
void __list_splice( list_node_t *list,
                    list_node_t *head ) ;

//11. 将一个链表连接到另一个链表后
void list_splice( list_node_t *list,
                  list_node_t *head );

//12. 获得结构体中某数据项的偏移量（宏）
#undef offsetof
#define offsetof(type, member) ((unsigned long)(&((type *)0)->member))

//13. 获得链表某一节点的地址（宏）
#define LIST_ENTRY(optr, type, member) \
        ((type *)((char *)(optr) - offsetof(type, member)))
        
//14. 查找某一节点（宏）
#define list_for_each( pos, head ) \
        for (pos = (head)->next; pos != (head); pos = pos->next)


//栈操作

#define stack_node_t list_node_t

//1. 初始化栈
#define INIT_STACK_HEAD INIT_LIST_HEAD

//2. 压栈
void stack_push( stack_node_t *head,
                 stack_node_t *node ) ;

//3. 弹栈
stack_node_t *stack_pop( stack_node_t *head ) ;

//队列操作

#define queue_node_t list_node_t

//1. 初始化队列
#define INIT_QUEUE_HEAD INIT_LIST_HEAD

//2. 加入队列(entry)
void list_enque( queue_node_t *head,
                 queue_node_t *node ) ;
        
//3. 取出队列元素(depart)
queue_node_t *list_deque( queue_node_t *head ) ;

//4. 重新加入队列(entry)
void list_enque_tail( queue_node_t *head,
                      queue_node_t *node ) ;
 
#endif

