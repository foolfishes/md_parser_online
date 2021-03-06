#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "../ThreadPool/ThreadPool.h"
#include "../Parser/Parser.h"


const int SERVER_PORT=1025;
//const char* SERVER_IP="127.0.0.1";
const int LISTEN_MAX = 36;
const size_t MAXEVENTS = 20;

void deal_fun(void* rfd)
{
  int* fd = (int*)rfd;
  char* buf = (char*)malloc(1024*1024);  //max file size: 1M
  std::string name;
  int readnum;
  //int namelen;
  readnum = read(*fd,buf, 1024*1024);
  if(readnum <=0)
  {
      close(*fd);
      //events[i].data.fd = -1;
      *fd = -1;
      rfd = (void*)fd;
      return;
  }
  int i=0;
  while(buf[i] !='\n')
  {
    name += buf[i++];
  }
  std::cout<<"read:\n" << buf;//<< std::endl;

  std::string infilename =  name;
  std::string outfilename = name.substr(0,name.find('.')) + ".html";
  std::ofstream fr(infilename.c_str());  //save the message to file 
  fr.write(buf+i, readnum-i);   //clip the filename
  fr.close();

  Mdfile file = Mdfile(infilename, outfilename);   //start parser
  Parser fileParser = Parser(&file);
  fileParser.start();

  std::fstream fw(outfilename.c_str());
  fw.seekg (0, fw.end);
  int length = fw.tellg();
  fw.seekg (0, fw.beg);
  memset(buf,0,1024*1024); //clear buff
  strcpy(buf,outfilename.c_str ());   //add file name
  strcat(buf,"\n");
  fw.read(buf+outfilename.size()+1, length);

  std::cout<<"converted:\n" << buf << std::endl;//<< std::endl;

  write(*fd,buf, length+outfilename.size()+1);
  free(buf);

  return;
}

int main()
{
  int listenfd, epollfd;
  struct sockaddr_in cliaddr, servaddr;
  struct epoll_event event;
  struct epoll_event *events;

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  // this machine
  servaddr.sin_port = htons(SERVER_PORT); //

  listenfd = socket(AF_INET, SOCK_STREAM, 0); //ipve4 tcp
  //no block socket
  int flags = fcntl (listenfd, F_GETFL, 0);
  flags |= O_NONBLOCK;
  fcntl (listenfd, F_SETFL, flags);
  //is needed ??
  bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
  listen(listenfd, LISTEN_MAX);

  events = (epoll_event *)calloc(MAXEVENTS, sizeof(event));
  event.data.fd = listenfd;
  event.events = EPOLLIN | EPOLLET;
  epollfd = epoll_create(3);   //3 is random no use
  epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&event);  //add fd to epool

  ThreadPool pool(4);   //create a threaad pool

  while(1)
  {
    int readyfds;
    readyfds = epoll_wait(epollfd, events, MAXEVENTS, -1);

    for(int i=0; i<readyfds; i++)
    {
      if(events[i].data.fd == listenfd)    //new connect
      {
        int connectfd;
        socklen_t addlen;
        connectfd = accept(listenfd, (sockaddr *)&cliaddr, &addlen);
        if(connectfd == -1)
        {
          LOG("connect erro\n");
          break;
        }
        char *str = inet_ntoa(cliaddr.sin_addr);
        std::cout << "accapt a connection from " << str << std::endl;

        event.data.fd = connectfd;
        event.events = EPOLLIN | EPOLLET;
        epoll_ctl(epollfd, EPOLL_CTL_ADD, connectfd, &event);
      }
      else    //data to read
      {
        int rfd = events[i].data.fd;
        if(rfd <0)
          continue;

        pool.addTask(Task(deal_fun, (void*)&rfd));   //add Task
        std::cout <<"pol size---------------" << pool.getPoolSize() << std::endl;

        if(rfd == -1)
        {
          events[i].data.fd = -1;
          std::cout << "deal failed" << std::endl;
        }
      }
    }
  }
  
  free (events);
  close (epollfd);
}