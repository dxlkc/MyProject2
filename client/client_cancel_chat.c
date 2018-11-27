#include "client.h"
//注销聊天
void cancel_chat(int sockfd)
{
    Msg msg;
    int flg;

    msg.cmd = CANCEL_CHAT;
    strncpy(msg.name, my_name, sizeof(my_name));

    flg = write(sockfd, &msg, sizeof(Msg));
    if (flg == -1)
    {
	printf("发送失败！\n");
    }
    sleep(1);
}
