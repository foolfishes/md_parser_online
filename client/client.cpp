#include "client.h"

const int BUF_SIZE = 1024*1024;

void str_cli(FILE* fp, int sockfd)
{
    char sendline[30], recvline[BUF_SIZE] = "";
    int maxfdpl, stdineof;
    int n;     //记录读取字节数
    fd_set rset;//select para

    stdineof = 0;
    FD_ZERO(&rset);
    printf("Input filename to parser:\n");
    for(;;)
    {
        if(stdineof == 0)
        {
            FD_SET(fileno(fp), &rset);
        }
        FD_SET(sockfd, &rset);   
        maxfdpl = (fileno(fp) > maxfdpl ? fileno(fp) : maxfdpl) + 1;
        select(maxfdpl, &rset, NULL, NULL, NULL);
        if(FD_ISSET(sockfd, &rset))  //socket is readable
        {
            if((n = read(sockfd, recvline, 1014*1024)) == 0)
            {
                if(stdineof == 1)
                {
                    printf("terminate normally\n");
                    return;
                }
                else
                {
                    printf("read error, server collapse!\n");//server collapse
                    exit(1);
                }
            }

            std::string filename;
            int i=0;
            while(recvline[i] != '\n')
            {
                filename += recvline[i++];
            } 
            std::ofstream fw(filename.c_str());
            if(fw.is_open())
                fw.write(recvline+i+1, n-i-1); //clip the filename

            printf("Get parsered file:%s\n", filename.c_str());
            printf("Input file to be send:\n");
        }
        if(FD_ISSET(fileno(fp), &rset)) //input is readable
        {
            if((n = read(fileno(fp), sendline, 30)) == 0)
            { 
                stdineof = 1;
                shutdown(sockfd, SHUT_WR);
                FD_CLR(fileno(fp), &rset);
                continue;
            }

            std::string buf="";
            if(!readFile(sendline, n, buf))
                write(sockfd, buf.c_str(), buf.size());
        }
    }
}

size_t readFile(char* sendline, int n, std::string& buf)
{
    char filename[30];
    memset(filename,0,30);
    memcpy(filename, sendline,n-1);   //n is the length of filename
        
    std::string tmp;
    buf += std::string(filename);
    buf += "\n";   // first line of buf is file name
    std::fstream file;
    file.open(filename);
    //printf("filename%s\n", filename);
    if(!file.is_open())
    {
        printf("file open erro\n");
        return -1;
    }
    while(getline(file,tmp))
    {
        buf += tmp;
        buf += "\n";
    }
    return 0;

}