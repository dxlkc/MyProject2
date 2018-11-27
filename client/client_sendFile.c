#include "client.h"
//读取
void *read_data(void *arg)
{
	int i=0,flg,ACK = 1;
	ARG_R *argu = (ARG_R*)arg; 
	int sockfd = argu->socket;
	Msg msg = argu->message;
	FILE *fp;

        //打开文件
	fp = fopen(msg.filename,"r");
	if (fp == NULL)
	{
		perror("文件打开失败!\n");
		exit(1);
	}
	while (1)
	{
	    usleep(50);
	    memset(msg.msg, 0, sizeof(msg.msg));
            //读取结束
            if (feof(fp))
            {
                msg.type = MSG_DONE;
            }
            else
            {
                fread(msg.msg, 1, sizeof(msg.msg), fp);
                if (ferror(fp) != 0)
                {
                    msg.type = MSG_EXCEPTION;
                    printf("读取文件失败！\n");
                    clearerr(fp);
		    break;
                }
                else
                {
                    msg.type = CONTENT;
                }
            }
	    //发送
	    flg = write(sockfd, &msg, sizeof(Msg));
   	    if (flg <= 0)
            {
                printf("发送失败！\n");
                return;
            }
	    if (msg.type == MSG_DONE)
	    {
		printf("文件发送成功！\n");
		break;
	    }
	    else if (msg.type == MSG_EXCEPTION)
	    {
		printf("文件读取错误！\n");
		break;
	    }

	    flg = read(sockfd, &ACK, sizeof(int));
	    if (flg == -1)
	    {
		printf("接收回复失败！\n");
	        return;
	    }
	    if (ACK == 0)
	    {
                printf("发送失败！\n");
                return;
	    }
	}
        fclose(fp);
}

//第二步 创建连接线程 
void sendFile(Msg msg)
{
    //创建socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("创建套接字失败!\n");
        return;
    }
    //初始化
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6000);
    addr.sin_addr.s_addr = inet_addr(msg.ip);
    //连接
    int flg = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if (flg == -1)
    {
        perror("和对方建立链接失败!\n");
        return;
    }

    flg = write(sockfd, &msg, sizeof(Msg));
    if (flg <= 0)
    {
	printf("发送失败！\n");
	return;
    }

    flg = read(sockfd, &msg, sizeof(Msg));
    if (flg <= 0)
    {
	printf("接收回复失败！\n");
	return;
    }

    if (msg.cmd == SENDFILE_REFUSE)
    {
	printf("对方拒绝接收！\n");
	close(sockfd);
        return;
    }
    //第三步 创建线程读取文件并发送
    pthread_t tid_send,tid_read;
    ARG_R arg;
    arg.message = msg;
    arg.socket = sockfd;
    pthread_create(&tid_read, NULL, read_data, &arg);
    pthread_detach(tid_read);
}

//第一步  发送传文件请求
void sendfile_request(int sockfd)
{
    Msg msg;
    int flg;
    char url[50];

    memset(&msg, 0, sizeof(Msg));
    strncpy(msg.fromname, my_name, strlen(my_name));
    printf("请输入文件路径 ：");
    scanf("%s",url);
    printf("请输入发送对象 ：");
    scanf("%s",msg.toname);

    msg.cmd = SENDFILE;
    msg.type = FILENAME;

    //存到msg.filename
    memcpy(msg.filename, url, strlen(url));
    flg = write(sockfd, &msg, sizeof(Msg));
    if (flg == -1)
    {
        printf("请求发送失败！\n");
    }
}

