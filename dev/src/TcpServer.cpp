#include"TcpServer.h"

TcpServer::TcpServer(const std::string &ip,const uint16_t port,int threadnum)
:threadnum_(threadnum),mainloop_(new EventLoop),acceptor_(mainloop_.get(),ip,port),threadpool_(threadnum,"IO")
{
  mainloop_->set_epoll_timeout(std::bind(&TcpServer::epoll_timeout,this,std::placeholders::_1));

  //acceptor_= new Acceptor(mainloop_.get(),ip,port);
  acceptor_.set_new_connection_callback(std::bind(&TcpServer::new_connection,this,std::placeholders::_1));

  for (int ii = 0; ii < threadnum_; ii++)
  {
    subloop_.emplace_back(new EventLoop);
    subloop_[ii]->set_epoll_timeout(std::bind(&TcpServer::epoll_timeout,this,std::placeholders::_1));
    threadpool_.addtask(std::bind(&EventLoop::run,subloop_[ii].get()));
  }


}
void TcpServer::start(){
  mainloop_->run();
}
TcpServer::~TcpServer()
{
  //delete mainloop_;
  //delete acceptor_;
  //for(auto &aa:conns_) delete aa.second;
  //for(auto &aa:subloop_) delete aa;
  //delete threadpool_;
}

void TcpServer::new_connection(std::unique_ptr<Socket> client_sock)//新客户端连接
{
  //spConnection conn=new Connection(mainloop_,client_sock);
  spConnection conn(new Connection(subloop_[client_sock->fd()%threadnum_].get(),std::move(client_sock)));
  conn->set_close_callback(std::bind(&TcpServer::close_connection,this,std::placeholders::_1));
  conn->set_error_callback(std::bind(&TcpServer::error_connection,this,std::placeholders::_1));
  conn->setonmessagecallback(std::bind(&TcpServer::onmessage,this,std::placeholders::_1,std::placeholders::_2));
  conn->set_complete_callback(std::bind(&TcpServer::send_complete,this,std::placeholders::_1));
  printf ("new connection(fd=%d,ip=%s,port=%d) ok.\n",conn->fd(),conn->ip(), conn->port());
  conns_[conn->fd()]=conn;
  if(newconnectioncb_)newconnectioncb_(conn);
}
void TcpServer::close_connection(spConnection conn){
  if(closeconnectioncb_)closeconnectioncb_(conn);
  //printf("1client(eventfd=%d) disconnected.\n",conn->fd());
  //close(conn->fd());            // 关闭客户端的fd。
  conns_.erase(conn->fd());
  //delete conn;
}
void TcpServer::error_connection(spConnection conn){
  if(errorconnectioncb_)errorconnectioncb_(conn);
  //printf("3client(eventfd=%d) error.\n",conn->fd());
  //close(conn->fd());            // 关闭客户端的fd。
  conns_.erase(conn->fd());
  //delete conn;
}
// 处理客户端的请求报文，在Connection类中回调此函数。
void TcpServer::onmessage(spConnection conn,std::string &message)
{
    // 在这里，将经过若干步骤的运算。
  if(onmessagecb_)onmessagecb_(conn,message);
}
void TcpServer::send_complete(spConnection conn)
{

  if(sendcompletecb_)sendcompletecb_(conn);
}
void TcpServer::epoll_timeout(EventLoop* loop)
{

  if(timeoutcb_) timeoutcb_(loop);
}
void TcpServer::setnewconnectioncb(std::function<void(spConnection )> fn)
{
    newconnectioncb_=fn;
}

void TcpServer::setcloseconnectioncb(std::function<void(spConnection )> fn)
{
    closeconnectioncb_=fn;
}

void TcpServer::seterrorconnectioncb(std::function<void(spConnection )> fn)
{
    errorconnectioncb_=fn;
}

void TcpServer::setonmessagecb(std::function<void(spConnection ,std::string &message)> fn)
{
    onmessagecb_=fn;
}

void TcpServer::setsendcompletecb(std::function<void(spConnection )> fn)
{
    sendcompletecb_=fn;
}

void TcpServer::settimeoutcb(std::function<void(EventLoop*)> fn)
{
    timeoutcb_=fn;
}
