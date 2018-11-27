#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysql.h"
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 7777
#define BACKLOG 10

#define REG_WANT 1                  //想要注册
#define ALTER_PSWD_WANT 2           //想要修改密码
#define LOGIN_WANT 3                //想要登录
#define PRI_CHAT 4                  //想要私聊
#define GROUP_CHAT 6                //群聊
#define DISPLAY  5                  //打印在线人员
#define CANCEL_CHAT 8               //注销聊天
#define SPEAK_NOT_WANT 7            //想禁言某人
#define SPEAK_NOT_BREAK_WANT 9      //解禁某人
#define KICK_WANT   20              //想踢出某人
#define SENDFILE 21                 //想传文件

#define REG_SUCCESS 10              //注册成功
#define REG_FAIL -1                 //注册失败
#define USER_NOT_EXIST -2           //用户不存在
#define ALTER_PSWD_FAIL -3          //修改密码失败
#define ALTER_PSWD_SUCCESS 11       //修改密码成功
#define LOGIN_SUCCESS 12            //成功登录
#define LOGIN_PSWD_FAIL -4          //密码错误
#define ANSWER_RIGHT 13             //密保问题回答正确
#define ANSWER_WRONG -5             //密保问题回答错误
#define SEND_FAIL -6                //信息发送失败
#define SEND_SUCCESS 15             //信息发送成功
#define HAD_LOGIN -7                //用户已登录
#define SEND_NOT_SPEAK -8           //被禁言不可发送
#define SEND_NOT_OUT -9             //被踢不可发送
#define CANCEL_SUCCESS 14           //注销成功
#define SEND_NOT_ONLINE -10         //对方不在线
#define SPEAK_NOT_SUCCESS 16        //禁言成功
#define SPEAK_NOT_BREAK 17          //解禁成功
#define KICK_SUCCESS 18             //踢出成功
#define NOT_GROUPOWNER -11          //不是群主
#define HAD_OUT  -13		    //已被踢出
#define USER_ONLINE -15		    //用户在线无法更改密码
#define OPPO_HAD_OUT -16	    //对方已被踢出

//群用
#define COMMON 0                    //默认值 可说、可看
#define OUT 1                       //被踢   不可说、不可看
#define SPEAK_NOT 2                  //禁言   不可说、可看

typedef struct messages
{
    char msg[2000];         // 消息内容
    char answer[100];      // 密保答案
    char name[20];          // 用户账号
    char password[20];      // 用户密码
    char toname[20];        // 发消息给某人
    char fromname[20];      // 收到某人的消息
    char filename[40];	    // 文件名
    char ip[20];	    // ip地址
    int cmd;                // 消息类型
    int type;               // 文件读取使用
}Msg;

typedef struct arg
{
    int client_socket;
    char client_ip[20];
}ARG;

typedef struct user
{
    char name[20];
    int sockfd;
    int cmd;
    char ip[20];
}user_info;

typedef struct NODE
{
    user_info data;
    struct NODE *next;
}Node;

MYSQL mysql;
Node *head;

void *handle(void* arg);
Node *create_list();
int insert_list(Node *head, user_info data);
int init_socket();
int Accept(int listen_socket);
void registe(int client_socket, Msg *msg);
void change_pswd(int client_socket, Msg *msg);
void cancel_chat(int client_sockfd, Msg *msg);
void display(int client_socket, Msg *msg);
void private_chat(int client_socket, Msg *msg);
void group_chat(int client_sockfd, Msg *msg);
int isGroupOwner(Msg *msg);
void banned(int client_sockfd, Msg *msg);
void banned_break(int client_sockfd, Msg *msg);
void kick(int client_sockfd, Msg *msg);
void sendFile(int client_socket, Msg *msg);

#endif // SERVER_H_INCLUDED

