#include"EventLoop.h"

/*
class EventLoop
{
private:
  Epoll ep_;
public:
  EventLoop();
  ~EventLoop();
  void run();
};
EventLoop::
*/

EventLoop::EventLoop():ep_(new Epoll)
{

}
EventLoop::~EventLoop()
{
  //delete ep_;
}
#include<unistd.h>
#include<sys/syscall.h>
void EventLoop::run()
{
  printf("thread is%d\n",syscall(SYS_gettid));
  while (true)        // 事件循环。
  {
      std::vector<Channel*>channels=ep_->loop();
      if(channels.size()==0){
        epoll_timeout_callback_(this);
      }{
        for (auto &ch:channels)       // 遍历epoll返回的数组evs。
        {
            ch->handle_event();
        }
      }
  }
}

/*Epoll* EventLoop::ep(){
  return ep_;
}*/
void EventLoop::update_channel(Channel *ch){
  ep_->update_channel(ch);
}
void EventLoop::set_epoll_timeout(std::function<void(EventLoop*)> fn)
{
  epoll_timeout_callback_=fn;
}
void EventLoop::remove_channel(Channel *ch)
{
  ep_->remove_channel(ch);
}
