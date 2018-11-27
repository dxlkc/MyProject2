#include "client.h"

//用户注册
void registe(int sockfd)
{
    Msg msg;
    int len, flg;
    int i, cnt_num, cnt_low, cnt_up, other;
    char name[20];
    char password[20];
    char second[20];

    msg.cmd = REG_WANT;
    printf("请输入用户名 : ");
    scanf("%s",name);
    while(1)
    {
        flg = 0;
        printf("请输入密码(包含大写字母、小写字母和数字、长度不小于8位) :\n");
        scanf("%s",password);

        len = strlen(password);
		cnt_num = cnt_low = cnt_up = other = 0;
		for(i=0; i<len; ++i)
		{
			if (password[i]>='0' && password[i]<='9')
				cnt_num++;
			else if (password[i]>='a' && password[i]<='z')
				cnt_low++;
			else if (password[i]>='A' && password[i]<='Z')
				cnt_up++;
			else
				other++;
		}
		if (cnt_num && cnt_low && cnt_up && !other && len>=8)
		{
		    i = 0;
            while(1)
            {
                i++;
                printf("请确认密码 : ");
                scanf("%s",second);

                if (strcmp(second,password) == 0)
                {
                    flg = 1;
                    break;
                }
                if (i == 3)
                {
                    printf("错误三次！请重新输入密码！\n");
                    break;
                }
                printf("不匹配！（请重新输入）\n");
            }
		}
		else
        {
            printf("该密码不符合要求！（请重新输入）\n");
        }
        if (flg)
        {
            break;
        }
    }
    getchar();
    printf("请输入密保问题 : ");
    gets(msg.msg);
    printf("请输入问题答案 : ");
    gets(msg.answer);

    strncpy(msg.name, name, sizeof(name));
    strncpy(msg.password, password, sizeof(password));

    write(sockfd, &msg, sizeof(Msg));
    read(sockfd, &msg, sizeof(Msg));
    sleep(1);
    switch(msg.cmd)
    {
        case REG_FAIL :
            printf("用户名已存在!\n");
            break;
        case REG_SUCCESS :
            printf("注册成功!\n");
            break;
    }
}
//修改密码
void change_pswd(int sockfd)
{
    Msg msg;
    char password[20];
    char answer[1024];
	char second[20];
    int len, flg;
    int i, cnt_num, cnt_low, cnt_up, other;

    msg.cmd = ALTER_PSWD_WANT;
    printf("请输入用户名 : ");
    scanf("%s",msg.name);

    write(sockfd, &msg, sizeof(msg));
    read(sockfd, &msg, sizeof(msg));

    if (msg.cmd == USER_NOT_EXIST)
    {
        printf("该用户名不存在！\n");
    }
    else if (msg.cmd == USER_ONLINE)
    {
	printf("该用户已在线！无法更改密码！\n");
    }
    else
    {
        read(sockfd, &msg, sizeof(msg));

        printf("请输入原密码 : ");
        scanf("%s",password);
        getchar();
        printf("(请回答密保问题) %s : ",msg.msg);
        gets(answer);

        if (strcmp(msg.password, password) == 0 && (strcmp(msg.answer, answer) == 0))
        {
            while(1)
            {
                flg = 0;
                printf("请输入新密码 : ");
                scanf("%s",password);
                len = strlen(password);
                cnt_num = cnt_low = cnt_up = other = 0;
                for(i=0; i<len; ++i)
                {
                    if (password[i]>='0' && password[i]<='9')
                        cnt_num++;
                    else if (password[i]>='a' && password[i]<='z')
                        cnt_low++;
                    else if (password[i]>='A' && password[i]<='Z')
                        cnt_up++;
                    else
                        other++;
                }
                if (cnt_num && cnt_low && cnt_up && !other && len>=8)
                {
                    i = 0;
                    while(1)
                    {
                        i++;
                        printf("请确认密码 : ");
                        scanf("%s",second);

                        if (strcmp(second,password) == 0)
                        {
                            flg = 1;
                            break;
                        }
                        if (i == 3)
                        {
                            printf("错误三次！请重新输入密码！\n");
                            break;
                        }
                        printf("不匹配！（请重新输入）\n");
                    }
                }
                else
                {
                    printf("该密码不符合要求！（请重新输入）\n");
                }
                if (flg)
                {
                    strncpy(msg.password, password, sizeof(password));
                    msg.cmd = ANSWER_RIGHT;
                    break;
                }
            }
        }
        else
        {
            printf("密码或密保答案错误！\n");
            msg.cmd = ALTER_PSWD_FAIL;
        }
        write(sockfd, &msg, sizeof(msg));
        read(sockfd, &msg, sizeof(msg));
        if (msg.cmd == ALTER_PSWD_SUCCESS)
        {
            printf("密码修改成功！\n");
        }
        else
        {
            printf("密码修改失败！\n");
        }
    }
}

