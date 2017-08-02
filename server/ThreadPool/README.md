# C++　Thread Pool
this is a Thread Pool write with C++, a well structed and tested.

## run a example
1. cmake; here i offer CMakeLists.txt file in the project, cmake it to get Makefile.
2. make; implement Makefile get the executable file: ThreadPool
3. ./ThreadPool; run the example

## Basic usage

1. Include the header in your source file: `#include "ThreadPool.h"`
2. Create a thread pool with number of threads you want: `ThreadPool pool(4);`
3. Add work to the pool: `pool.addTask(Task(task1));`,Task is a class for task to run;

The workers(threads) will start their work automatically as fast as there is new workin the pool. 
If you want to destroy the pool you can use `pool.destroy();`.

## features 
- minimum thread num in pool is 4, ant it is also the default number
- if the waiting task is more than twice of thread in pool, it will add new thread to pool, the number is half of pool size 
- a thread will wait 60s if no task to run, after that it will exit automaticlly
- if you destroy the pool it will destroy until all task is finished 

## optimization
Your comments are welcome

foolfish >_<