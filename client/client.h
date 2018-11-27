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
#define REG_WANT 1                  //��Ҫע��
#define ALTER_PSWD_WANT 2           //��Ҫ�޸�����
#define LOGIN_WANT 3                //��Ҫ��¼
#define PRI_CHAT 4                  //��Ҫ˽��
#define GROUP_CHAT 6                //Ⱥ��
#define DISPLAY  5                  //��ӡ������Ա
#define CANCEL_CHAT 8               //ע������
#define SPEAK_NOT_WANT 7            //�����ĳ��
#define SPEAK_NOT_BREAK_WANT 9      //���ĳ��
#define KICK_WANT   20              //���߳�ĳ��
#define SENDFILE 21            //�봫�ļ�

#define REG_SUCCESS 10              //ע��ɹ�
#define REG_FAIL -1                 //ע��ʧ��
#define USER_NOT_EXIST -2           //�û�������
#define ALTER_PSWD_FAIL -3          //�޸�����ʧ��
#define ALTER_PSWD_SUCCESS 11       //�޸�����ɹ�
#define LOGIN_SUCCESS 12            //�ɹ���¼
#define LOGIN_PSWD_FAIL -4          //�������
#define ANSWER_RIGHT 13             //�ܱ�����ش���ȷ
#define ANSWER_WRONG -5             //�ܱ�����ش����
#define SEND_FAIL -6                //��Ϣ����ʧ��
#define SEND_SUCCESS 15             //��Ϣ���ͳɹ�
#define HAD_LOGIN -7                //�û��ѵ�¼
#define SEND_NOT_SPEAK -8           //�����Բ��ɷ���
#define SEND_NOT_OUT -9             //���߲��ɷ���
#define CANCEL_SUCCESS 14           //ע���ɹ�
#define SEND_NOT_ONLINE -10         //�Է�������
#define SPEAK_NOT_SUCCESS 16        //���Գɹ�
#define SPEAK_NOT_BREAK 17          //����ɹ�
#define KICK_SUCCESS 18             //�߳��ɹ�
#define NOT_GROUPOWNER -11          //����Ⱥ��
#define HAD_OUT  -13		    //�ѱ��߳�
#define SENDFILE_REFUSE -14	    //�ܾ������ļ�
#define USER_ONLINE -15		    //�û������޷���������
#define OPPO_HAD_OUT -16	    //�Է��ѱ��߳�

//���ļ���
#define BUFSIZE 20               //������С
#define FILENAME 1               //�ļ���
#define CONTENT 2                //�ļ�����
#define MSG_DONE 3               //��ȡ����
#define MSG_EXCEPTION 4          //��ȡ����
#define SENDFILE_YES 5		 //ͬ�����

typedef struct messages
{
    char msg[2000];         // ��Ϣ����
    char answer[100];      // �ܱ���
    char name[20];          // �û��˺�
    char password[20];      // �û�����
    char toname[20];        // ����Ϣ��ĳ��
    char fromname[20];      // �յ�ĳ�˵���Ϣ
    char filename[40];	    // �ļ���
    char ip[20];	    // ip��ַ
    int cmd;                // ��Ϣ����
    int type;               // �ļ���ȡʹ��
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




