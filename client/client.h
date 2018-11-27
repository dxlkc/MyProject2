#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <pthread.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <semaphore.h>

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
#define SENDFILE 21            //想传文件

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
#define SENDFILE_REFUSE -14	    //拒绝接收文件
#define USER_ONLINE -15		    //用户在线无法更改密码
#define OPPO_HAD_OUT -16	    //对方已被踢出

//传文件用
#define BUFSIZE 20               //容器大小
#define FILENAME 1               //文件名
#define CONTENT 2                //文件内容
#define MSG_DONE 3               //读取结束
#define MSG_EXCEPTION 4          //读取错误
#define SENDFILE_YES 5		 //同意接收

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

typedef struct pmsg
{
    Msg message;
    int socket;
}ARG_R;

void *recvMsg(void *sockfd);
void registe(int sockfd);
void change_pswd(int client_socket);
void menu();
void login(int sockfd);
void call_server(int sockfd);
void cancel_chat(int sockfd);
void display(int sockfd);
void private_chat(int sockfd);
void chat_menu();
void sendMsg(int sockfd);
void group_chat(int sockfd);
void banned(int sockfd);
void banned_break(int sockfd);
void kick(int sockfd);
void sendfile_request(int sockfd);
void sendFile(Msg msg);
void *file_connect(void* message);
void *read_data(void *arg);
void *recvFile();

char my_name[20];


#endif // CLIENT_H_INCLUDED




