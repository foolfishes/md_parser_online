#include "server.h"

const int SERVER_PORT=1025;
const int LISTEN_MAX = 36;
const size_t MAXEVENTS = 20;

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
  event.events = EPOLLIN;
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
        event.events = EPOLLIN;
        epoll_ctl(epollfd, EPOLL_CTL_ADD, connectfd, &event);
      }
      else    //data to read
      {
        int rfd = events[i].data.fd;
        if(rfd <0)
          continue;

        pool.addTask(Task(deal_fun, (void*)&rfd));   //add Task

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
