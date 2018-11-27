#include "client.h"
//私聊
void private_chat(int sockfd)
{
    Msg msg;
    int flg;

    msg.cmd = PRI_CHAT;

    printf ("请输入发送的对象 : ");
    scanf("%s",msg.toname);
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

