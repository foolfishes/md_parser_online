#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
const int SERVER_PORT=1025;

void str_cli(FILE* fp, int sockfd);  //处理函数声明

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

void str_cli(FILE* fp, int sockfd)
{
    char sendline[30], recvline[1024*1024] = "";
    int maxfdpl, stdineof;
    int n;     //记录读取字节数
    fd_set rset;//select para

    stdineof = 0;
    FD_ZERO(&rset);
    printf("Input file to be send:\n");
    for(;;)
    {
        if(stdineof == 0)
        {
            FD_SET(fileno(fp), &rset);
        }
        FD_SET(sockfd, &rset);   //参数设置
        maxfdpl = (fileno(fp) > maxfdpl ? fileno(fp) : maxfdpl) + 1;
        select(maxfdpl, &rset, NULL, NULL, NULL);//调用select
        if(FD_ISSET(sockfd, &rset))  //socket is readable
        {//处理socket口
            if((n = read(sockfd, recvline, 1014*1024)) == 0)
            {
                if(stdineof == 1)
                {
                    printf("terminate normally\n");
                    return;
                }
                else
                {//服务器异常退出
                    printf("read error, server collapse!\n");//server collapse
                    exit(1);
                }
            }
            printf("receive:\n");
            cout << recvline<< endl;
            string filename;
            int i=0;
            while(recvline[i] != '\n')
            {
                filename += recvline[i++];
            } 
            cout << "filename:" << filename<< endl;
            ofstream fw(filename.c_str());
            if(fw.is_open())
                fw.write(recvline+i+1, n-i-1); //clip the filename
            printf("Input file to be send:\n");
        }
        if(FD_ISSET(fileno(fp), &rset)) //input is readable
        {//处理标准输入口
            if((n = read(fileno(fp), sendline, 30)) == 0)
            {   //客户端发起终止连接
                stdineof = 1;
                shutdown(sockfd, SHUT_WR);
                printf("client-shutdown\n");
                FD_CLR(fileno(fp), &rset);
                continue;
            }
            
            char* filename = (char*)malloc(30);
            memset(filename,0,30);
            memcpy(filename, sendline,n-1);
        
            string tmp,buf="";
            buf += string(filename);
            buf += "\n";   // first line of buf is file name
            fstream file;
            file.open(filename);
            if(file.is_open())
            {
                cout << "open\n";
            }
            else
                cout << "erro\n";
            while(getline(file,tmp))
            {
                buf += tmp;
                buf += "\n";
            }

            //printf("%s\n", buf.c_str());
            cout << "input file size: " << buf.size() << endl;
            //write(sockfd, filename, buf.size());
            write(sockfd, buf.c_str(), buf.size());
        }
    }
}