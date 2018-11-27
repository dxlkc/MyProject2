#include "client.h"

int main(int argc, char* argv[])
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket error!\n");
        exit(1);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    int flg = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if (flg == -1)
    {
        perror("connect error!\n");
        exit(1);
    }
    call_server(sockfd);

    close(sockfd);
    return 0;
}
