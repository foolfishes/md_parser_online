#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_
#include <pthread.h>
#include <unistd.h>
#include <deque>
#include <vector>
#include <list>
#include "Task.h"
#include "SynThread.h"
#include "Debug.h"

const int POOL_ALIVE= 1;
const int POOL_DESTROY= 0;
const size_t MIN_POOL_SIZE = 4;    //min threadpool size

class ThreadPool
{
public:
  ThreadPool(size_t size = 4);
  ~ThreadPool();

  void destroy();  //destroy pool
  size_t addTask(const Task& task);    //add Task to queue, 0 for true, 1 for false;
  size_t getNumTask();  //get number of task in queue
  size_t getPoolSize();
  size_t setPoolsize(size_t size);
  size_t getPoolState();
  void addThread();
  Mutex& getTaskMutex();
  Mutex& getSizeMutex();
  Mutex& getThreadMutex();
  SynThread& getTaskCond();
  std::deque<Task>& getTaskQue();
  std::list<pthread_t>& getThreadid();
  std::list<pthread_t>& getThreadidWork();


private:
  size_t m_pool_size;    //how many thread in the pool 
  size_t m_pool_state;    //0 for destroy, 1 for alive
  Mutex m_task_mutex;   //for visit m_task
  Mutex m_poolsize_mutex;   //for visit m_pool_size
  Mutex m_thread_mutex;   //for visit m_threadid and m_threadid_work
  SynThread m_task_cond;
  std::deque<Task> m_task;
  std::list<pthread_t> m_threadid;    //thread waiting
  std::list<pthread_t> m_threadid_work;    //thread that are working not waiting

};

#endif