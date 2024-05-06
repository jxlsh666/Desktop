#pragma once
#include"EventLoop.h"
#include"Socket.h"
#include"Channel.h"
#include"InerAddress.h"
#include"Buffer.h"
#include<functional>
#include<memory>
#include<atomic>
class Connection;
using spConnection=std::shared_ptr<Connection>;
class Connection: public std::enable_shared_from_this<Connection>
{
private:

  EventLoop *loop_;         //客户端
  std::unique_ptr<Socket> client_sock_;
  std::unique_ptr<Channel> client_channel_;
  std::function<void(spConnection)> close_callback_;      //fd_close回调TCPserver里的函数
  std::function<void(spConnection)> error_callback_;
  Buffer input_buffer_;
  Buffer output_buffer_;
  std::function<void(spConnection,std::string&)> onmessagecallback_;   // 处理报文的回调函数，将回调TcpServer::onmessage()。
  std::function<void(spConnection)> send_complete_callback_;   // 处理报文的回调函数，将回调TcpServer::onmessage()。
  std::atomic_bool disconnect_ ;
public:
  Connection(EventLoop *loop,std::unique_ptr<Socket> client_sock);
  ~Connection();
  int fd()const;
  std::string ip() const;
  uint16_t port() const;
  void close_callback();
  void on_message();
  void error_callback();
  void write_callback();//处理写事件
  void set_close_callback(std::function<void(spConnection)> fn);
  void set_error_callback(std::function<void(spConnection)> fn);
  void set_complete_callback(std::function<void(spConnection)> fn);
  void setonmessagecallback(std::function<void(spConnection,std::string&)> fn);    // 设置处理报文的回调函数。
  void send(const char *data,size_t size);

};


