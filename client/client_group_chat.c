#include "client.h"

//群聊
void group_chat(int sockfd)
{
    Msg msg;
    int flg;

    msg.cmd = GROUP_CHAT;

    strncpy(msg.fromname, my_name, sizeof(my_name));
    getchar();
    printf ("请输入要发送的内容 : \n");
    fgets(msg.msg, sizeof(msg.msg), stdin);

    flg = write(sockfd, &msg, sizeof(Msg));
    if (flg == -1)
    {
        printf("发送失败！\n");
    }
}

//禁言
void banned(int sockfd)
{
    Msg msg;
    int flg;

    msg.cmd = SPEAK_NOT_WANT;
    strncpy(msg.name, my_name, sizeof(my_name));

    printf("请输入需要被禁言的用户 : ");
    scanf("%s",msg.toname);

    flg = write(sockfd, &msg, sizeof(Msg));
    if (flg == -1)
    {
        printf("发送失败！\n");
    }
}

//解禁
void banned_break(int sockfd)
{
    Msg msg;
    int flg;

    msg.cmd = SPEAK_NOT_BREAK_WANT;
    strncpy(msg.name, my_name, sizeof(my_name));

    printf("请输入需要被解禁的用户 : ");
    scanf("%s",msg.toname);

    flg = write(sockfd, &msg, sizeof(Msg));
    if (flg == -1)
    {
        printf("发送失败！\n");
    }
}

//踢人
void kick(int sockfd)
{
    Msg msg;
    int flg;

    msg.cmd = KICK_WANT;
    strncpy(msg.name, my_name, sizeof(my_name));

    printf("请输入需要被踢出的用户 : ");
    scanf("%s",msg.toname);

    flg = write(sockfd, &msg, sizeof(Msg));
    if (flg == -1)
    {
        printf("发送失败！\n");
    }
}








