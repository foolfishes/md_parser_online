#ifndef _MUTEX_H_
#define _MUTEX_H_
#include <pthread.h>
#include "Debug.h"

class Mutex
{
public:
  Mutex();
  ~Mutex();

  void mutexLock();
  void mutexUnlock();
  void mutexTrylock();
  pthread_mutex_t* getMutex();

private:
  pthread_mutex_t m_lock;
};


#endif