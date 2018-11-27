#include "server.h"
//私聊
void private_chat(int client_socket, Msg *msg)
{
    printf ("%s 发送一条消息给 %s\n", msg->fromname, msg->toname);

    int flg;
    Node *tmp = head->next;
    //找fromname
    while(tmp)
    {
        if (strcmp(tmp->data.name, msg->fromname) == 0)
        {
            if (tmp->data.cmd == SPEAK_NOT)      //禁言中
            {
                msg->cmd = SEND_NOT_SPEAK;
                break;
            }
            else if (tmp->data.cmd == OUT)        //被踢
            {
                msg->cmd = SEND_NOT_OUT;
                break;
            }
            else
            {
                tmp = head->next;
                while(tmp)
                {
                    if (strcmp(tmp->data.name, msg->toname) == 0)
                    {
			if (tmp->data.cmd != OUT)
			{
                        	flg = write(tmp->data.sockfd, msg, sizeof(Msg));
                        	if (flg != -1)
                        	{
                            		msg->cmd = SEND_SUCCESS;
                        	}
                        	else
                        	{
                            		msg->cmd = SEND_FAIL;
                        	}
			}
			else
			{
				msg->cmd = OPPO_HAD_OUT;
			}
			break;
                    }
                    tmp = tmp->next;
                }
                if (tmp == NULL)
                {
                    msg->cmd = SEND_NOT_ONLINE;
                }
                break;
            }
        }
        tmp = tmp->next;
    }
    write(client_socket, msg, sizeof(Msg));
}


