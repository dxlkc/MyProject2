#include "server.h"

int init_socket()   
{
    int flg;
    //创建socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("socket error!\n");
		exit(1);
	}
	//初始化
	struct sockaddr_in seraddr;
    seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(PORT);
	seraddr.sin_addr.s_addr = INADDR_ANY;

	int opt = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	//绑定
	flg = bind(sockfd, (struct sockaddr*)&seraddr, sizeof(struct sockaddr));
	if (flg == -1)
	{
		perror("bind error!\n");
		exit(1);
	}
	//监听
	flg = listen(sockfd,BACKLOG);
	if (flg == -1)
	{
		perror("listen error!\n");
		exit(1);
	}
	printf("wait.....\n");
	return sockfd;
}

void login(int client_socket, Msg *msg, char ip[])
{
    user_info Data;
    int flg;
    char info_query[100];
    char password[20];
    MYSQL_RES *res;
    MYSQL_ROW row;

    printf("%s  正在登录...\n",msg->name);
    sprintf(info_query,"select * from user_info where name='%s'",msg->name);    //查找是否有该用户
    flg = mysql_real_query(&mysql,info_query,strlen(info_query));
    if (flg)
    {
        printf("failed to query:%s\n",mysql_error(&mysql));
    }
    else
    {
        res = mysql_store_result(&mysql);
        row = mysql_fetch_row(res);
        if (NULL == row)
        {
            printf("%s 账号不存在!\n",msg->name);
            msg->cmd = USER_NOT_EXIST;                      //不存在该用户
        }
        else
        {
            Node *tmp = head->next;
            flg = 0;
            while(tmp)
            {
                if (strcmp(tmp->data.name, msg->name) == 0)
                {
                    flg = 1;
                    break;
                }
		tmp = tmp->next;
            }
            if (flg)                                                 //判断是否已登录
            {
                msg->cmd = HAD_LOGIN;
            }
            else
            {
                if (strcmp(row[1], msg->password) == 0)
                {
                    printf("%s 登录成功\n",msg->name);
                    msg->cmd = LOGIN_SUCCESS;
                    strncpy(Data.name, msg->name, sizeof(msg->name));
		    strncpy(Data.ip, ip, strlen(ip));
                    Data.sockfd = client_socket;
                    Data.cmd = COMMON;
                    insert_list(head,Data);
                }
                else
                {
                    printf("%s 密码错误!\n",msg->name);
                    msg->cmd = LOGIN_PSWD_FAIL;
                }
            }
        }
    }
    flg = write(client_socket, msg, sizeof(Msg));
    if (flg == -1)
    {
	printf("发送失败！\n");
    }
}

void *handle(void* arg)
{
    ARG argu = *(ARG*)arg;
    int flg, client_socket;
    Msg msg;
    char ip[20];

    client_socket = argu.client_socket;
    strncpy(ip, argu.client_ip, strlen(argu.client_ip));
	
    while(1)
    {
        flg = read(client_socket, &msg, sizeof(Msg));
        if (flg == -1)
        {
            perror("read error!\n");
            break;
        }
        else if (flg == 0)   //表示客户端断开连接（退出）
        {
	    cancel_chat(client_socket, &msg);
            printf("%s 已退出\n",ip);
            break;
        }

        switch(msg.cmd)
        {
            case REG_WANT :                      // 客户端进行注册
                registe(client_socket, &msg);
                break;
            case LOGIN_WANT :                    // 客户端进行登录
                login(client_socket, &msg, ip);
                break;
            case ALTER_PSWD_WANT :               //修改密码
                change_pswd(client_socket, &msg);
                break;
            case DISPLAY :			             //打印在线人员
                display(client_socket, &msg);
                break;
            case CANCEL_CHAT :			        //注销
                cancel_chat(client_socket, &msg);
		break;
            case PRI_CHAT :			             //私聊
                private_chat(client_socket, &msg);
                break;
            case GROUP_CHAT :                    //群聊
                group_chat(client_socket, &msg);
		break;
            case SPEAK_NOT_WANT :                   //禁言
                banned(client_socket, &msg);
		break;
            case SPEAK_NOT_BREAK_WANT :             //解禁
                banned_break(client_socket, &msg);
		break;
            case KICK_WANT :                        //踢人
                kick(client_socket, &msg);
                break;
	    case SENDFILE :
		sendFile(client_socket, &msg);
		break;
            default :
                break;
        }
    }
    close(client_socket);
}

