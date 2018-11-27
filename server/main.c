#include "server.h"


Node *head = NULL;
/*连接到数据库*/
void connection(const char* host, const char* user, const char* password, const char* database)
{
	mysql_init(&mysql);
	if(mysql_real_connect(&mysql,host,user,password,database,0,NULL,0))
    {
        printf("Connection success!\n");
    }
    else
    {
        printf("Connection failed!\n");
        printf("Insert error %d: %s\n", mysql_errno(&mysql), mysql_error(&mysql));
        exit(1);
    }
}

int main()
{
    connection("localhost","root","123","chat_db");
    head = create_list();
    int listen_socket = init_socket();

    while(1)
    {
	ARG arg;
	struct sockaddr_in cliaddr;
	int client_socket,len;                  //client_socket是通信用的
	len = sizeof(struct sockaddr_in);

	client_socket = accept(listen_socket, (struct sockaddr*)&cliaddr, &len);
	if (client_socket == -1)
        {
            perror("accept error!\n");
            exit(1);
        }
        printf ("a client connect : %s\n", inet_ntoa(cliaddr.sin_addr));
	
	memset(&arg, 0, sizeof(arg));
	strncpy(arg.client_ip, inet_ntoa(cliaddr.sin_addr),strlen(inet_ntoa(cliaddr.sin_addr)));
	arg.client_socket = client_socket;

        pthread_t tid;
        pthread_create(&tid, NULL, handle, (void *)&arg);

        pthread_detach(tid);
    }

    close(listen_socket);
    return 0;
}

