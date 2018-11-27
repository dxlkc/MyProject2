#include "server.h"

//用户注册
void registe(int client_socket, Msg *msg)
{
    int flg;
    char info_query[100];        //对user_info进行的操作
    printf("%s 正在注册...\n",msg->name);
    sprintf(info_query,"insert into user_info value ('%s','%s','%s','%s')",msg->name,msg->password,msg->msg,msg->answer);
    flg = mysql_real_query(&mysql,info_query,strlen(info_query));
    if (flg)
    {
        printf("failed to query:%s\n",mysql_error(&mysql));
        msg->cmd = REG_FAIL;                                   //失败说明已存在该用户名
    }
    else
    {
        printf("add success!\n");
        msg->cmd = REG_SUCCESS;                                //注册成功
    }
    write(client_socket, msg, sizeof(Msg));

}
//修改密码
void change_pswd(int client_socket, Msg *msg)
{
    int flg;
    char info_query[100];        //对user_info进行的操作
    MYSQL_RES *res;
    MYSQL_ROW row;

    printf("%s  正在修改密码...\n",msg->name);

    Node *tmp = head->next;
    while(tmp)
    {
        if (strcmp(tmp->data.name, msg->name) == 0)
        {
	    msg->cmd = USER_ONLINE;
	    write(client_socket, msg, sizeof(Msg));
	    return;
        }
        tmp = tmp->next;
    }         
    sprintf(info_query,"select * from user_info where name='%s'",msg->name);    
    flg = mysql_real_query(&mysql,info_query,strlen(info_query));
    if (flg)
    {
        printf("failed to query:%s\n",mysql_error(&mysql));
    }
    else
    {
        res = mysql_store_result(&mysql);
        row = mysql_fetch_row(res);
        if(NULL == row)
        {
            printf("%s 账号不存在!\n",msg->name);
            msg->cmd = USER_NOT_EXIST;                   
        }                                                
        write(client_socket, msg, sizeof(Msg));

        if (msg->cmd == USER_NOT_EXIST)
        {
            return ;
        }
        else
        {
            strncpy(msg->password, row[1], sizeof(msg->password));
            strncpy(msg->msg, row[2], sizeof(msg->msg));
            strncpy(msg->answer, row[3], sizeof(msg->answer));

            write(client_socket, msg, sizeof(Msg));
            read(client_socket, msg, sizeof(Msg));

            if (msg->cmd == ANSWER_RIGHT)
            {
                sprintf(info_query,"update user_info set password='%s' where name='%s'",msg->password,msg->name);
                flg = mysql_real_query(&mysql,info_query,strlen(info_query));
                if (flg)
                {
                    printf("failed to query:%s\n",mysql_error(&mysql));
                    msg->cmd = ALTER_PSWD_FAIL;
                }
                else
                {
                    printf("%s 修改密码成功!\n",msg->name);
                    msg->cmd = ALTER_PSWD_SUCCESS;
                }
            }
	    else
	    {
		printf("修改密码失败！\n");
		msg->cmd = ALTER_PSWD_FAIL;
	    }
        }
    }
    write(client_socket, msg, sizeof(Msg));
}

