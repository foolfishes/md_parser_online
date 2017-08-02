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

const int BUF_SIZE = 1024*1024;

void deal_fun(void* rfd);