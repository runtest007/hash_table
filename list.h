#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>

//�������

//����Ľڵ�
typedef struct list_node
{
        struct list_node *prev;
        struct list_node *next;
}list_node_t;

//1. ��ʼ�������꣩
#define INIT_LIST_HEAD(ptr) do { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

//zxj
#define INIT_LIST_NODE(ptr) do { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)


//2. ���������һ���ڵ㣨������
void _list_add( list_node_t *prev,
                list_node_t *next,
                list_node_t *new_node ) ;

//3. ��ͷ�ڵ�����һ�ڵ�
void list_add_head( list_node_t *head,
                    list_node_t *new_node ) ;

//4. �������β�����һ���ڵ�
void list_add_tail( list_node_t *head,
                    list_node_t *new_node ) ;


//5. ɾ��һ���ڵ㣨��������ʹ��ɾ���ڵ��ǰ���ͺ��
void list_del( list_node_t *prev, 
               list_node_t *next );

//6��ɾ��һ���ڵ㣨��������ʹ��ɾ���ڵ�
void list_del_node( list_node_t *entry ) ;

//7. ��һ�������еĽڵ��ƶ�����һ�������е�ͷ�ڵ��
void list_move( list_node_t *note,
                list_node_t *head );

//8. ��һ�������еĽڵ��ƶ�����һ�������е�β��
void list_move_tail( list_node_t *note, 
                     list_node_t *head ) ;

//9. �ж�һ�������Ƿ�Ϊ��
int list_empty( const list_node_t *head ) ;

//10�� ��һ���������ӵ���һ������� Ϊ��������
void __list_splice( list_node_t *list,
                    list_node_t *head ) ;

//11. ��һ���������ӵ���һ�������
void list_splice( list_node_t *list,
                  list_node_t *head );

//12. ��ýṹ����ĳ�������ƫ�������꣩
#undef offsetof
#define offsetof(type, member) ((unsigned long)(&((type *)0)->member))

//13. �������ĳһ�ڵ�ĵ�ַ���꣩
#define LIST_ENTRY(optr, type, member) \
        ((type *)((char *)(optr) - offsetof(type, member)))
        
//14. ����ĳһ�ڵ㣨�꣩
#define list_for_each( pos, head ) \
        for (pos = (head)->next; pos != (head); pos = pos->next)


//ջ����

#define stack_node_t list_node_t

//1. ��ʼ��ջ
#define INIT_STACK_HEAD INIT_LIST_HEAD

//2. ѹջ
void stack_push( stack_node_t *head,
                 stack_node_t *node ) ;

//3. ��ջ
stack_node_t *stack_pop( stack_node_t *head ) ;

//���в���

#define queue_node_t list_node_t

//1. ��ʼ������
#define INIT_QUEUE_HEAD INIT_LIST_HEAD

//2. �������(entry)
void list_enque( queue_node_t *head,
                 queue_node_t *node ) ;
        
//3. ȡ������Ԫ��(depart)
queue_node_t *list_deque( queue_node_t *head ) ;

//4. ���¼������(entry)
void list_enque_tail( queue_node_t *head,
                      queue_node_t *node ) ;
 
#endif

