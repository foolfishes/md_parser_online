#ifndef DEBUG_H
#define DEBUG_H
#include <stdio.h>

#define LOG(...) {printf(__VA_ARGS__);}
#define RUN_LOG(...) //{printf("file[%s]line[%u]func[%s]: ",__FILE__,__LINE__,__FUNCTION__); printf(__VA_ARGS__);}
#define DEBUG(...) //{printf(__VA_ARGS__);}

#endif