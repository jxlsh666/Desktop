#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threadnum,std::string thread_type):stop_(false),thread_type_(thread_type)
{
  // 启动threadnum个线程，每个线程将阻塞在条件变量上。
	for (size_t ii = 0; ii < threadnum; ii++)
    {
      // 用lambda函创建线程。
		  threads_.emplace_back([this]
      {
        printf("create%s thread(%ld).\n",thread_type_.c_str(),(SYS_gettid));     // 显示线程ID。
        std::cout << "子线程：" << std::this_thread::get_id() << std::endl;
			  while (stop_==false)
			  {
				  std::function<void()> task;       // 用于存放出队的元素。
				  {
            // 锁作用域的开始。 ///////////////////////////////////
					  std::unique_lock<std::mutex> lock(this->mutex_);
					  // 等待生产者的条件变量。
					  this->condition_.wait(lock, [this]
            {
              return ((this->stop_==true) || (this->taskqueue_.empty()==false));
            });
            // 在线程池停止之前，如果队列中还有任务，执行完再退出。
					  if ((this->stop_==true)&&(this->taskqueue_.empty()==true)) return;
            // 出队一个任务。
					  task = std::move(this->taskqueue_.front());
					  this->taskqueue_.pop();
				  }   // 锁作用域的结束。 ///////////////////////////////////
          printf("%sthread is %d.\n",thread_type_.c_str(),syscall(SYS_gettid));
				  task();  // 执行任务。
			  }
		  });
    }
}

void ThreadPool::addtask(std::function<void()> task)
{
    {   // 锁作用域的开始。 ///////////////////////////////////
      std::lock_guard<std::mutex> lock(mutex_);
      taskqueue_.push(task);
    }   // 锁作用域的结束。 ///////////////////////////////////
    condition_.notify_one();   // 唤醒一个线程。
}

ThreadPool::~ThreadPool()
{
	stop_ = true;

	condition_.notify_all();  // 唤醒全部的线程。

    // 等待全部线程执行完任务后退出。
	for (std::thread &th : threads_)
        th.join();
}
size_t ThreadPool::size(){
  return threads_.size();
}

// g++ -o test ThreadPool.cpp -lpthread
