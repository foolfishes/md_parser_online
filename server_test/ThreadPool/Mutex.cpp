#include "Mutex.h"

Mutex::Mutex()
{
  if(pthread_mutex_init(&m_lock, NULL))
    LOG("mutex init failed\n");
}

Mutex::~Mutex()
{
  if(pthread_mutex_destroy(&m_lock))
    LOG("mutex destroy failed\n");
}

void Mutex::mutexLock()
{

  if(pthread_mutex_lock(&m_lock))
    LOG("mutes lock failed\n");
}
void Mutex::mutexUnlock()
{
  if(pthread_mutex_unlock(&m_lock))
    LOG("mutex unlock faild\n");

}
void Mutex::mutexTrylock()
{
  if(pthread_mutex_trylock(&m_lock))
    LOG("mutex trylock failed\n");
}

pthread_mutex_t* Mutex::getMutex()
{
  return &m_lock;
}
