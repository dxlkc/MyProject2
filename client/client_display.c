#include "client.h"
//打印在线人员
void display(int sockfd)
{
    Msg msg;
    int flg;

    msg.cmd = DISPLAY;
    write(sockfd, &msg, sizeof(Msg));
    printf("在线人员：\n");
    sleep(2);
}
