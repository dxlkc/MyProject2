#include "client.h"

void chat_menu()
{
    printf("\n-------- Chatting Room --------\n");
    printf("      1. 查询在线人员\n");
    printf("      2. 私聊\n");
    printf("      3. 群聊\n");
    printf("      4. 踢人\n");
    printf("      5. 禁言\n");
    printf("      6. 解禁\n");
    printf("      7. 发送文件\n");
    printf("      8. 注销\n");
    printf("-------------------------------\n\n");
}

void sendMsg(int sockfd)
{
    int n;
    pthread_t tid;
    while(1)
    {
        chat_menu();
        printf("请选择功能（输入序号）: ");
        scanf("%d",&n);
        switch(n)
        {
            case 1:
                display(sockfd);
                break;
            case 2:
                private_chat(sockfd);
                break;
            case 3:
                group_chat(sockfd);
                break;
            case 4:
                kick(sockfd);
                break;
            case 5:
                banned(sockfd);
                break;
            case 6:
                banned_break(sockfd);
                break;
            case 7:
		sendfile_request(sockfd);
                break;
            case 8:
                cancel_chat(sockfd);
                return;
            default:
                break;
        }
    }
}

//接收信息
void *recvMsg(void *fd)
{
    Msg msg;
    int sockfd = *(int*)fd;
    while(1)
    {
        read(sockfd, &msg, sizeof(msg));
        switch(msg.cmd)
        {
            case PRI_CHAT:
                printf("\n%s 发来信息 ：\n --> %s\n",msg.fromname, msg.msg);
                break;
            case GROUP_CHAT:
                printf("\n（群消息）%s 发来信息 ：\n --> %s\n",msg.fromname, msg.msg);
                break;
            case DISPLAY:
                printf("  %s\n",msg.name);
                break;
            case CANCEL_SUCCESS:
                printf("注销成功！\n");
                return;
            case SEND_NOT_SPEAK:
                printf("您被禁言！无法发送信息！\n");
                break;
            case SEND_NOT_OUT:
                printf("您被踢出群聊！无法发送信息！\n");
                break;
            case SEND_NOT_ONLINE:
                printf("对方不在线！\n");
                break;
            case SEND_FAIL:
                printf("发送失败！\n");
                break;
            case NOT_GROUPOWNER:
                printf("您不是群主，没有该权限！\n");
                break;
	    case HAD_OUT:
		printf("该用户已被踢出群！无法操作！\n");
		break;
            case SPEAK_NOT_SUCCESS:
                printf("禁言成功！\n");
                break;
            case SPEAK_NOT_BREAK:
                printf("解禁成功！\n");
                break;
            case KICK_SUCCESS:
                printf("踢出成功！\n");
                break;
            case SENDFILE :
                sendFile(msg);
                break;
	    case OPPO_HAD_OUT :
		printf("对方被踢，无法发送！\n");
		break;
            default:
                break;
        }
    }
}

