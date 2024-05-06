#pragma once
#include"Epoll.h"
#include"functional"
#include<memory>
class Epoll;
class Channel;
class EventLoop
{
private:
  std::unique_ptr<Epoll> ep_;
  std::function<void(EventLoop*)> epoll_timeout_callback_;//超时回调函数
public:
  EventLoop(/* args */);
  ~EventLoop();
  void run();
  void update_channel(Channel *ch);
  void remove_channel(Channel *ch);
  //Epoll* ep();
  void set_epoll_timeout(std::function<void(EventLoop*)> fn);
};
