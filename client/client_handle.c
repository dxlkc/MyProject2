#include "client.h"
//菜单
void menu()
{
    printf("-------- Welcome --------\n");
    printf("    1. 注册\n");
    printf("    2. 登录\n");
    printf("    3. 修改密码\n");
    printf("    4. 退出\n");
    printf("-------------------------\n\n");
}

//登录
void login(int sockfd)
{
    Msg msg;
    char name[20];
    char password[20];
    int flg;

    msg.cmd = LOGIN_WANT;
    printf("请输入用户名 : ");
    scanf("%s",name);
    printf("请输入密码 : ");
    scanf("%s",password);

    strncpy(msg.name, name, sizeof(name));
    strncpy(msg.password, password, sizeof(password));

    flg = write(sockfd, &msg, sizeof(Msg));
    if (flg == -1)
    {
	printf("发送失败！\n");
    }
    flg = read(sockfd, &msg, sizeof(Msg));
    if (flg == -1)
    {
	printf("接收失败！\n");
    }
    if (msg.cmd == LOGIN_SUCCESS)
    {
        printf("登录成功!\n");
        strncpy(my_name, name, sizeof(name));

        pthread_t tid_recv;
        pthread_create(&tid_recv, NULL, recvMsg, (void*)&sockfd);
        pthread_detach(tid_recv);

        sendMsg(sockfd);
    }
    else if (msg.cmd == LOGIN_PSWD_FAIL)
    {
        printf("登录失败!(密码错误)\n");
    }
    else if (msg.cmd == USER_NOT_EXIST)
    {
        printf("登录失败!(用户名不存在)\n");
    }
    else if (msg.cmd == HAD_LOGIN)
    {
        printf("登录失败!(用户已在线)\n");
    }
}

//用户选择
void call_server(int sockfd)
{
    int n;
    while(1)
    {
        menu();
        printf("请选择功能（输入序号）: ");
        scanf("%d",&n);
        switch(n)
        {
            case 1:
                registe(sockfd);
                break;
            case 2:
                login(sockfd);
                break;
            case 3:
                change_pswd(sockfd);
                break;
            case 4:
            default:
                exit(1);
        }
    }
}

