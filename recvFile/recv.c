#include "recv_file.h"

void *recvfile(void* arg)
{
	int flg,i=0,ACK;
	ARG *argu = (ARG*)arg;
	int file_sockfd = argu->socket;
	FILE *fp = argu->p;
	Msg msg;
	while(1)
	{
		flg = read(file_sockfd, &msg, sizeof(Msg));
		if (flg <= 0)
		{
			printf("接收失败!\n");
			ACK = 0;
			flg = write(file_sockfd, &ACK, sizeof(int));
   	    		if (flg < 0)
            		{
               			printf("回复失败！\n");
                		return;
            		}
			break;
		}
	    	if (msg.type == MSG_DONE)
	    	{
			printf("接收成功！\n");
			break;
	    	}
		else if (msg.type == MSG_EXCEPTION)
		{
			printf("文件损坏！\n");
			break;
		}
		if (msg.type == CONTENT)
		{
			flg = fwrite(msg.msg, 1, strlen(msg.msg), fp);
			if (flg < 0)
			{
				ACK = 0;
				flg = write(file_sockfd, &ACK, sizeof(int));
   	    			if (flg < 0)
            			{
               				printf("回复失败！\n");
                			break;
            			}
				printf("接收失败！\n");
				break;
			}
			ACK = 1;
			flg = write(file_sockfd, &ACK, sizeof(int));
   	    		if (flg < 0)
            		{
               			printf("回复失败！\n");
                		break;
            		}
		}
		else
		{
			ACK = 0;
			flg = write(file_sockfd, &ACK, sizeof(int));
   	    		if (flg < 0)
            		{
               			printf("回复失败！\n");
                		return;
            		}
		}
	}
	fclose(fp);
	close(file_sockfd);
}

int main()
{
    int flg;
    //创建socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("创建套接字失败!\n");
		return(1);
	}
	//初始化
	struct sockaddr_in seraddr;
        seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(6000);
	seraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int opt = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	//绑定
	flg = bind(sockfd, (struct sockaddr*)&seraddr, sizeof(struct sockaddr));
	if (flg == -1)
	{
		perror("绑定失败!\n");
		return;
	}
	//监听
	flg = listen(sockfd,BACKLOG);
	if (flg == -1)
	{
		perror("监听失败!\n");
		return;
	}
	while(1)
    {
        //接收连接
        struct sockaddr_in cliaddr;
        int file_socket,len;                  //flie_socket是文件通信用的
        len = sizeof(struct sockaddr_in);

        file_socket = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
        if (sockfd == -1)
        {
            perror("接受连接失败!\n");
            return;
        }
        printf ("a client connect : %s\n", inet_ntoa(cliaddr.sin_addr));

        /**************************************/

		Msg msg;
		char my_url[50],ch;
		char from_url[30];
		FILE *fp,*p;

		flg = read(file_socket, &msg, sizeof(Msg));
		if (flg <= 0)
		{
			printf("接收失败！\n");
			return;
		}
		if (strrchr(msg.filename,'/') == NULL)
		{
		    strncpy(from_url, msg.filename, sizeof(msg.filename));
		}
		else
		{
		    strncpy(from_url,strrchr(msg.filename,'/')+1,strlen(strrchr(msg.filename,'/')+1));
		}
		printf("%s 发来文件 %s，是否接收？(y/n) : ",msg.fromname,from_url);
		scanf("%s",&ch);
		if (ch == 'y')
		{
		    printf("请输入存放路径（以'/'为结尾）: ");
		    scanf("%s",my_url);
		    strcat(my_url, from_url);
		    fp = fopen(my_url, "w");
		    if (fp == NULL)
		    {
				printf("文件打开失败！\n");
				return;
		    }

			msg.cmd = SENDFILE_YES;
			strncpy(msg.toname, msg.fromname, strlen(msg.fromname));
			strncpy(msg.fromname, my_name, strlen(my_name));
			flg = write(file_socket, &msg, sizeof(Msg));
			if (flg <= 0)
			{
				printf("回复失败！\n");
			}
			else
			{
				printf("开始接收！\n");
				ARG arg;
				arg.socket = file_socket;
				arg.p = fp;
				pthread_t tid_recv;
				pthread_create(&tid_recv, NULL, recvfile, &arg);
				pthread_detach(tid_recv);
			}

		}
		else
		{
			msg.cmd = SENDFILE_REFUSE;
			strncpy(msg.toname, msg.fromname, strlen(msg.fromname));
			strncpy(msg.fromname, my_name, strlen(my_name));
			printf("拒绝接收！\n");
			flg = write(file_socket, &msg, sizeof(Msg));
			if (flg <= 0)
			{
				printf("回复失败！\n");
			}
			else
			{printf("\nerror!\n");}
		}
    }
  	close(sockfd);
}

