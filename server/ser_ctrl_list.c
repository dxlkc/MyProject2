#include "server.h"
//创建链表   ok
Node *create_list()
{
    Node *list = (Node*)malloc(sizeof(Node));
    if (!list)
    {
        return NULL;
    }
    list->next = NULL;
    return list;
}

//插入元素   ok
int insert_list(Node *head, user_info Data)
{
    if (head == NULL)
    {
        return 0;
    }

    Node *node = (Node*)malloc(sizeof(Node));
    node->data = Data;
    node->next = NULL;

    Node *tmp = head;
    while(tmp->next)
    {
        tmp = tmp->next;
    }
    tmp->next = node;
    return 1;
}

