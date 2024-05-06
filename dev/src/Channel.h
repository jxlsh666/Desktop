#pragma once

//#include"Epoll.h"
#include <sys/epoll.h>
#include<functional>
#include"InerAddress.h"
#include"Socket.h"
#include"EventLoop.h"

class EventLoop;
class Channel
{
private:
  /* data */
  int fd_=-1;         //Channel唯一fd
  EventLoop*loop_=nullptr;  //对应的红黑树，多对一Epoll
  bool inepoll_=false;// 是否在树上
  uint32_t events_=0; //监视事件
  uint32_t revents_=0; //已发生事件
  //bool islisten_=false;
  std::function<void()> read_callback_;       //fd_读事件的回调函数，如果是acceptchannel，将回调Acceptor::newconnection()，如果是clientchannel，将回调Channel::onmessage()。
  std::function<void()> close_callback_;      //fd_close回调Connection里的函数
  std::function<void()> error_callback_;      //fd_error回调Connection里的函数
  std::function<void()> write_callback_;
public:
  Channel (EventLoop *loop,int fd);
  ~Channel();
  int fd();
  void use_et();          //边缘触发
  void enable_reading();  //监视读事件
  void disable_reading();  //取消监视读事件
  void enable_writing();  //监视写事件
  void disable_writing();  //取消监视读事件
  void disable_all();     //取消事件
  void remove();
  void set_in_epoll(bool inepoll);
  void set_revents(uint32_t ev);
  void handle_event();
  void set_read_callback(std::function<void()> fn);
  void set_close_callback(std::function<void()> fn);
  void set_error_callback(std::function<void()> fn);
  void set_write_callback(std::function<void()> fn);
  bool in_poll();
  uint32_t events();
  uint32_t revents();

};

