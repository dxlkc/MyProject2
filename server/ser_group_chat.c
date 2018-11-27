#include "server.h"

//群聊
void group_chat(int client_sockfd, Msg *msg)
{
    Node *tmp = head->next;
    int flg;

    printf("%s 想群发信息: %s\n",msg->fromname,msg->msg);

    while(tmp)
    {
	if (strcmp(tmp->data.name,msg->fromname) == 0)
	{
	    if (tmp->data.cmd != COMMON)
	    {
	    	if (tmp->data.cmd == OUT)
	    	{
		    msg->cmd = SEND_NOT_OUT;
		    printf("该用户被踢，无法发送！\n");
           	}
	        else if (tmp->data.cmd == SPEAK_NOT)
	        {
	            msg->cmd = SEND_NOT_SPEAK;
		    printf("该用户被禁，无法发送！\n");
	        }
		flg = write(client_sockfd, msg, sizeof(Msg));
                if (flg == -1)
                {
                    printf("回复失败！\n");
                }
		else printf("end\n");
		return;
	    }
	    else
	    {
		break;
            }
	}
	tmp = tmp->next;
    }
    tmp = head->next;
    while(tmp)
    {
        if (tmp->data.cmd != OUT && strcmp(tmp->data.name,msg->fromname) != 0)
        {
            flg = write(tmp->data.sockfd, msg, sizeof(Msg));
            if (flg == -1)
            {
                printf("发送给 %s 失败！\n",tmp->data.name);
            }
        }
        tmp = tmp->next;
    }
}

//是否为群主
int isGroupOwner(Msg *msg)
{
    Node *tmp = head->next;
    if (strcmp(tmp->data.name, msg->name) == 0)
    {
        return 1;
    }
    return 0;
}

//禁言
void banned(int client_sockfd, Msg *msg)
{
    Node *tmp = head->next;
    int flg;

    flg = isGroupOwner(msg);
    if (flg != 1)
    {
        msg->cmd = NOT_GROUPOWNER;
	printf("该用户不是群主!\n");
    }
    else
    {
        tmp = tmp->next;
        while(tmp)
        {
            if (strcmp(tmp->data.name, msg->toname) == 0)
            {
		if (tmp->data.cmd == OUT)       //对象被已经踢
		{
		    msg->cmd = HAD_OUT;
		}
		else
		{
               	    tmp->data.cmd = SPEAK_NOT;
                    msg->cmd = SPEAK_NOT_SUCCESS;
		}
                break;
            }
            tmp = tmp->next;
        }
        if (tmp == NULL)
        {
            msg->cmd = SEND_NOT_ONLINE;
        }
    }
    flg = write(client_sockfd, msg, sizeof(Msg));
    if (flg == -1)
    {
        printf("回复失败！\n");
    }
}

//解禁
void banned_break(int client_sockfd, Msg *msg)
{
    Node *tmp = head->next;
    int flg;

    flg = isGroupOwner(msg);
    if (flg != 1)
    {
        msg->cmd = NOT_GROUPOWNER;
    }
    else
    {
        tmp = tmp->next;
        while(tmp)
        {
            if (strcmp(tmp->data.name, msg->toname) == 0)
            {
		if (tmp->data.cmd == OUT)
		{
		    msg->cmd = HAD_OUT;
		}
		else
		{
                   tmp->data.cmd = COMMON;
                   msg->cmd = SPEAK_NOT_BREAK;
		}
                break;
            }
            tmp = tmp->next;
        }
        if (tmp == NULL)
        {
            msg->cmd = SEND_NOT_ONLINE;
        }
    }
    flg = write(client_sockfd, msg, sizeof(Msg));
    if (flg == -1)
    {
        printf("回复失败！\n");
    }
}

//踢人
void kick(int client_sockfd, Msg *msg)
{
    Node *p, *tmp = head->next;
    int flg;

    p = head;
    flg = isGroupOwner(msg);
    if (flg != 1)
    {
        msg->cmd = NOT_GROUPOWNER;
    }
    else
    {
        tmp = tmp->next;
        while(tmp)
        {
            if (strcmp(tmp->data.name, msg->toname) == 0)
            {
		if (tmp->data.cmd == OUT)
		{
		    msg->cmd = HAD_OUT;
		}
		else
		{
                   tmp->data.cmd = OUT;
                   msg->cmd = KICK_SUCCESS;
		}
                break;
            }
            tmp = tmp->next;
        }
        if (tmp == NULL)
        {
            msg->cmd = SEND_NOT_ONLINE;
        }
    }
    flg = write(client_sockfd, msg, sizeof(Msg));
    if (flg == -1)
    {
        printf("回复失败！\n");
    }
}

