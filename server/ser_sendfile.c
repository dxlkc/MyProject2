#include "server.h"
//转发接收方ip
void sendFile(int client_socket, Msg *msg)
{
    Node *p, *tmp = head->next;
    int flg;

    while(tmp)
    {
        if (strcmp(tmp->data.name, msg->toname) == 0)
        {
            strncpy(msg->ip, tmp->data.ip, strlen(tmp->data.ip));
            flg = write(client_socket, msg, sizeof(Msg));
            if (flg == -1)
            {
                printf("发送失败！\n");
            }
            break;
        }
        tmp = tmp->next;
    }
    if (tmp == NULL)
    {
        msg->cmd =  SEND_NOT_ONLINE;
        flg = write(client_socket, msg, sizeof(Msg));
        if (flg == -1)
        {
            printf("发送失败！\n");
        }
    }
}

