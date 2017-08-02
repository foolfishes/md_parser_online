#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>


void str_cli(FILE* fp, int sockfd); 
size_t readFile(char* sendline, int n, std::string& buf);