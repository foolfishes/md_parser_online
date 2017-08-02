#ifndef _SYNTHREAD_H_
#define _SYNTHREAD_H_
#include <pthread.h>
#include "Debug.h"
#include "Mutex.h"


class SynThread
{
public:
  SynThread(Mutex& lock);
  ~SynThread();
  void synSignal();
  void synWait();
  void synWaitTime();
  void synBroadcast();

private:
  SynThread(const SynThread&);    //no definition
  SynThread& operator=(SynThread&);
  Mutex& m_mutex;
  pthread_cond_t m_cond;
};

#endif