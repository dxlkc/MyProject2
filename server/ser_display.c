#include "server.h"
//打印在线人员
void display(int client_socket, Msg *msg)
{
    Node *tmp = head->next;
    int flg;

    while(tmp)
    {
        strncpy(msg->name, tmp->data.name, sizeof(tmp->data.name));
        flg = write(client_socket, msg, sizeof(Msg));
        if (flg == -1)
	{
	    printf("发送失败！\n");
	    break;
	}
	sleep(1);
        tmp = tmp->next;
    }
}
