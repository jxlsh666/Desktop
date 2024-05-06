#pragma once
#include"EventLoop.h"
#include"Socket.h"
#include"Channel.h"
#include"InerAddress.h"
#include<functional>
#include<memory>
class Acceptor
{
private:
  EventLoop *loop_;         //
  Socket serv_sock_;
  Channel accept_channel_;
  std::function<void(std::unique_ptr<Socket>)> new_connection_callback_;
public:
  Acceptor(EventLoop *loop,const std::string &ip,const uint16_t port);
  ~Acceptor();
  void new_connection();//新客户端连接
  void set_new_connection_callback(std::function<void(std::unique_ptr<Socket>)> fn);
};


