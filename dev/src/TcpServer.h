#pragma once
#include"EventLoop.h"
#include"Socket.h"
#include"Channel.h"
#include"Acceptor.h"
#include"Connection.h"
#include<map>
#include"ThreadPool.h"
#include<memory>
//Tcp网络服务类
class TcpServer
{
private:
  std::unique_ptr<EventLoop> mainloop_;
  std::vector<std::unique_ptr<EventLoop>> subloop_;                           //从事件
  Acceptor acceptor_;
  int threadnum_;
  ThreadPool threadpool_;                                    //线程池

  std::map<int,spConnection> conns_;
  std::function<void(spConnection)> newconnectioncb_;          // 回调EchoServer::HandleNewConnection()。
  std::function<void(spConnection)> closeconnectioncb_;        // 回调EchoServer::HandleClose()。
  std::function<void(spConnection)> errorconnectioncb_;         // 回调EchoServer::HandleError()。
  std::function<void(spConnection,std::string &message)> onmessagecb_;        // 回调EchoServer::HandleMessage()。
  std::function<void(spConnection)> sendcompletecb_;            // 回调EchoServer::HandleSendComplete()。
  std::function<void(EventLoop*)>  timeoutcb_;                       // 回调EchoServer::HandleTimeOut()。

public:
  TcpServer(const std::string &ip,const uint16_t port,int threadnum=3);
  ~TcpServer();
  void start();
  void new_connection(std::unique_ptr<Socket> client_sock);//新客户端连接
  void close_connection(spConnection conn);
  void error_connection(spConnection conn);
  void onmessage(spConnection conn,std::string& message);     // 处理客户端的请求报文，在Connection类中回调此函数。
  void send_complete(spConnection conn);
  void epoll_timeout(EventLoop* loop);
  void setnewconnectioncb(std::function<void(spConnection)> fn);
  void setcloseconnectioncb(std::function<void(spConnection)> fn);
  void seterrorconnectioncb(std::function<void(spConnection)> fn);
  void setonmessagecb(std::function<void(spConnection,std::string &message)> fn);
  void setsendcompletecb(std::function<void(spConnection)> fn);
  void settimeoutcb(std::function<void(EventLoop*)> fn);
};

