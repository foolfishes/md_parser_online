#include "SynThread.h"

SynThread::SynThread(Mutex& lock):m_mutex(lock)
{
  if(pthread_cond_init(&m_cond,NULL))
    LOG("pthread_cond_init failed!\n");

}

SynThread::~SynThread()
{
  if(pthread_cond_destroy(&m_cond))
    LOG("pthread_cond_destory failed\n");
}

//?????? nee to mutex
void SynThread::synSignal()
{
  if(pthread_cond_signal(&m_cond))
    LOG("pthread_cond_signal failed\n");

  RUN_LOG("signal %d\n",pthread_self());
}

void SynThread::synBroadcast()
{
  if(pthread_cond_broadcast(&m_cond))
    LOG("pthread_cond_broadcast failed\n");

  RUN_LOG("broadcast %d\n",pthread_self());
}

void SynThread::synWait()
{
  RUN_LOG("wait %d\n",pthread_self());
  if(pthread_cond_wait(&m_cond, m_mutex.getMutex()))
    LOG("pthread_cond_wait failed\n");

  RUN_LOG("wake up %d\n",pthread_self());
}

void SynThread::synWaitTime()
{
  struct timespec ts;
  ts.tv_sec = time(NULL) + 60;  //time to wait: 60s
  ts.tv_nsec = 0;
  pthread_cond_timedwait(&m_cond, m_mutex.getMutex(), &ts);

}
