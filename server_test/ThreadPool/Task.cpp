#include "Task.h"

Task::Task(const FunPtr& func, void* args):m_fun(func),arg(args){}

Task::~Task(){}

void Task::run()
{
  m_fun(arg);
}