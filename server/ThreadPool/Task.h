#ifndef _TASK_H_
#define _TASK_H_

class Task
{
public:
  typedef void (*FunPtr)(void*);     //no argument void( *funptre)(void *arg)
  explicit Task(const FunPtr& func, void* args);
  ~Task();
  void run();
private:
  FunPtr m_fun;
  void* arg;

};


#endif