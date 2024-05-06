#include"Acceptor.h"




Acceptor::Acceptor(EventLoop *loop,const std::string &ip,const uint16_t port)
      :serv_sock_(create_non_blocking()),accept_channel_(loop_,serv_sock_.fd())
{
  loop_=loop;

  InerAddress servaddr(ip,port); //服务端地址与协议
  serv_sock_.set_keep_alive(1);
  serv_sock_.set_reuse_port(1);
  serv_sock_.set_reuse_addr(1);
  serv_sock_.set_tcp_nodelay(1);
  serv_sock_.bind(servaddr);
  serv_sock_.listen();
  //accept_channel_=new Channel(loop_,serv_sock_.fd());
  //服务端只会添加新客户端，所以绑定new_connection
  accept_channel_.set_read_callback(std::bind(&Acceptor::new_connection,this));

  accept_channel_.enable_reading();

}
Acceptor::~Acceptor(){
  //delete accept_channel_;
}

//#include"Connection.h"

void Acceptor:: new_connection()//新客户端连接
{
  InerAddress client_addr;
  std::unique_ptr<Socket> client_sock(new Socket(serv_sock_.accept(client_addr)));
  //printf ("accept client(fd=%d,ip=%s,port=%d) ok.\n",client_sock->fd(),client_addr.ip(),client_addr.port());
  client_sock->set_ipport(client_addr.ip(),client_addr.port());
  new_connection_callback_(std::move(client_sock));
}
void Acceptor::set_new_connection_callback(std::function<void(std::unique_ptr<Socket>)> fn)
{
  new_connection_callback_=fn;
}
