#include "ThreadPool.h"

static void* workThread(void* arg)    //global function
{
  ThreadPool* pool = static_cast<ThreadPool*>(arg);
  while(pool->getPoolState() == POOL_ALIVE)
  {
    pool->getTaskMutex().mutexLock();

    if((pool->getPoolState() == POOL_ALIVE) && pool->getTaskQue().empty())
     {
       pool->getTaskCond().synWaitTime();  //wait for waking up
     }

    if(pool->getPoolState() == POOL_DESTROY)
    {
      pool->getTaskMutex().mutexUnlock();
      pthread_exit(NULL);
      return NULL;
    }
    if(!pool->getTaskQue().empty())
    {
      Task task = pool->getTaskQue().front();
      pool->getTaskQue().pop_front();
      pool->getTaskMutex().mutexUnlock();

      pool->getThreadMutex().mutexLock();  //add the thread to work list
      pool->getThreadidWork().push_back(pthread_self());
      pool->getThreadid().remove(pthread_self());
      pool->getThreadMutex().mutexUnlock();

      task.run();  //should unlock first then to run, or it will unlock untill run finish

      pool->getThreadMutex().mutexLock();  //add the thread to idle list
      pool->getThreadid().push_back(pthread_self());
      pool->getThreadidWork().remove(pthread_self());
      pool->getThreadMutex().mutexUnlock();
    }
    else     // need to deatroy idle thread
    {
      pool->getTaskMutex().mutexUnlock(); // unlock

      pool->getSizeMutex().mutexLock();
      pool->getThreadMutex().mutexLock(); 
      size_t psize = pool->getPoolSize();     //pool size
      size_t idlesize =  pool->getThreadid().size();

      if((psize > MIN_POOL_SIZE) && (psize/2 < idlesize))   //too much idle thread;
      {
        //LOG("psize: %ul, idlesize: %d\n", psize, idlesize);
        pool->getThreadid().remove(pthread_self());
        pool->setPoolsize(psize-1);

        pool->getSizeMutex().mutexUnlock();
        pool->getThreadMutex().mutexUnlock(); 

        DEBUG("kill a thread\n");

        if(pthread_detach(pthread_self()))
          LOG("detach erro\n");
        pthread_exit(NULL);
        return NULL;
      }
      pool->getSizeMutex().mutexUnlock();
      pool->getThreadMutex().mutexUnlock(); 
    }
        
  }
  return NULL;
}



ThreadPool::ThreadPool(size_t size):
  m_pool_size(size),
  m_poolsize_mutex(),
  m_thread_mutex(),
  m_task_cond(m_task_mutex)  //m_synchronizer need to init?
{
  m_pool_state = POOL_ALIVE;

  pthread_t threadid;
  for(size_t i=0; i<m_pool_size; i++)
  {
    if(pthread_create(&threadid, NULL, workThread, this))
      LOG("thread create failed!\n");
    m_threadid.push_back(threadid);

  }

}

ThreadPool::~ThreadPool()
{
  if(m_pool_state != POOL_DESTROY)
  {
      destroy();
  }
}


size_t ThreadPool::addTask(const Task& task)
{

  m_task_mutex.mutexLock();
  m_task.push_back(task);
  m_task_cond.synSignal();   //wake one thread

  addThread();   //if needed, add more thead;
  
  m_task_mutex.mutexUnlock(); 

  return 0;

}

size_t ThreadPool::getNumTask()
{
  size_t num;
  m_task_mutex.mutexLock();
  num = m_task.size();
  m_task_mutex.mutexUnlock();
  return num;
}

void ThreadPool::destroy()
{
  while((!m_threadid_work.empty()) || (!m_task.empty()))
  {
    sleep(1);   //wait for all task finish;
  }

  m_pool_state = POOL_DESTROY;   
  m_task_cond.synBroadcast();

  while(!m_threadid.empty())
  {
    DEBUG("final pool size :%lu\n", m_threadid.size());
    pthread_join(m_threadid.back(),NULL);
    m_threadid.pop_back();
  }

}

void ThreadPool::addThread()
{
  m_poolsize_mutex.mutexLock();
  m_thread_mutex.mutexLock(); 
  if(m_pool_size*2 < m_task.size())   //too much idle thread; number of task is two times of pool size at most
  {
    pthread_t threadid;
    //DEBUG("task size %d , pool size %d\n", m_task.size(), m_pool_size);
    for(size_t i=0; i<(m_pool_size/2); i++)
    {
      if(pthread_create(&threadid, NULL, workThread, this))
        LOG("thread create failed!\n");
      m_threadid.push_back(threadid);
    }
    m_pool_size += m_pool_size/2;
    DEBUG("new pool size: %lu\n", m_pool_size);

  }
  m_thread_mutex.mutexUnlock();
  m_poolsize_mutex.mutexUnlock();
  return;

}


size_t ThreadPool::getPoolSize()
{
  return m_pool_size;
}

size_t ThreadPool::setPoolsize(size_t size)
{
  m_pool_size = size;
  return 0;
}


size_t ThreadPool::getPoolState()
{
  return m_pool_state;
}

Mutex& ThreadPool::getTaskMutex()
{
  return m_task_mutex;
}

std::deque<Task>& ThreadPool::getTaskQue()
{
  return m_task;
}

SynThread& ThreadPool::getTaskCond()
{
  return m_task_cond;
}

Mutex& ThreadPool::getSizeMutex()
{
  return m_poolsize_mutex;
}

Mutex& ThreadPool::getThreadMutex()
{
  return m_thread_mutex;
}

std::list<pthread_t>& ThreadPool::getThreadid()
{
  return m_threadid;
}

std::list<pthread_t>& ThreadPool::getThreadidWork()
{
  return m_threadid_work;
}
