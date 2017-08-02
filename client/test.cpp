
#include "client.h"

using namespace std;
const int SERVER_PORT=1025;

int main(int argc, char** argv)
{
    int sockfd;
    struct sockaddr_in servaddr;
    const char* ips = "127.0.0.1";
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    if(inet_aton(ips, &servaddr.sin_addr) <= 0)
    {
        printf("inet_aton error!\n");
        exit(0);
    }
    if(connect(sockfd, (struct sockaddr*)& servaddr, sizeof(servaddr)) < 0)
    {
        printf("connect error!\n");
        exit(0);
    }
    
    str_cli(stdin, sockfd);
    exit(0);
}

