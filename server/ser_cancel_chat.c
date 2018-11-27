#include "server.h"
//注销聊天
void cancel_chat(int client_socket, Msg *msg)
{
    Node *tmp,*p;
    int flg = 0;

    tmp = head;
    p = tmp->next;
    while(p)
    {
        if (strcmp(p->data.name, msg->name) == 0)
        {
            flg = 1;
            tmp->next = p->next;
            free(p);
	    break;
        }
	p = p->next;
	tmp = tmp->next;
    }
    if (flg)
    {
        msg->cmd = CANCEL_SUCCESS;
	printf("%s 注销成功！\n",msg->name);
    }
    flg = write(client_socket, msg, sizeof(Msg));
    if (flg == -1)
    {
	printf("发送失败！\n");
    }
}
