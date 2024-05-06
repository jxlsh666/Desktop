#include"Socket.h"
int create_non_blocking(){
  // 创建服务端用于监听的listenfd。
    int listen_fd = socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK,IPPROTO_TCP);
    if (listen_fd < 0)
    {
        perror("socket() failed");
        exit(-1);
    }
    return listen_fd;
}

    // 创建服务端用于监听的listenfd。
    /*   int listenfd = socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK,IPPROTO_TCP);
    if (listenfd < 0)
    {
        perror("socket() failed"); return -1;
    }

    设置listenfd的属性，如果对这些属性不熟悉，百度之。
    int opt = 1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,static_cast<socklen_t>(sizeof opt));    // 必须的。
    setsockopt(listenfd,SOL_SOCKET,TCP_NODELAY   ,&opt,static_cast<socklen_t>(sizeof opt));    // 必须的。
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEPORT ,&opt,static_cast<socklen_t>(sizeof opt));    // 有用，但是，在Reactor中意义不大。
    setsockopt(listenfd,SOL_SOCKET,SO_KEEPALIVE   ,&opt,static_cast<socklen_t>(sizeof opt));    // 可能有用，但是，建议自己做心跳。

    setnonblocking(listenfd);    // 把服务端的listenfd设置为非阻塞的。

    struct sockaddr_in servaddr;                                  // 服务端地址的结构体。
    servaddr.sin_family = AF_INET;                              // IPv4网络协议的套接字类型。
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);      // 服务端用于监听的ip地址。
    servaddr.sin_port = htons(atoi(argv[2]));               // 服务端用于监听的端口。




    if (bind(listenfd,servaddr.addr(),sizeof(servaddr)) < 0 )
    {
        perror("bind() failed"); close(listenfd); return -1;
    }

    if (listen(listenfd,128) != 0 )        // 在高并发的网络服务器中，第二个参数要大一些。
    {
        perror("listen() failed"); close(listenfd); return -1;
    }
    */

int Socket::fd(){
  return fd_;
}
void Socket::set_reuse_port(bool on) {
  int opt=on?1:0;
  ::setsockopt(fd_,SOL_SOCKET,SO_REUSEPORT ,&opt,static_cast<socklen_t>(sizeof opt));    // 有用，但是，在Reactor中意义不大。

}
void Socket::set_keep_alive(bool on) {
  int opt=on?1:0;
  ::setsockopt(fd_,SOL_SOCKET,SO_KEEPALIVE   ,&opt,sizeof(opt));    // 可能有用，但是，建议自己做心跳。

}
void Socket::set_reuse_addr(bool on) {
    int opt=on?1:0;
  ::setsockopt(fd_,SOL_SOCKET,SO_REUSEADDR ,&opt,sizeof(opt));    // 有用，但是，在Reactor中意义不大。

}

void Socket::set_tcp_nodelay(bool on) {
  int opt=on?1:0;
  ::setsockopt(fd_,IPPROTO_TCP,TCP_NODELAY ,&opt,sizeof(opt));    // 有用，但是，在Reactor中意义不大。

}

void Socket::bind(const InerAddress &serv_addr) {
  if (::bind(fd_,serv_addr.addr(),sizeof(serv_addr)) < 0 )
  {
      perror("bind() failed");
      close(fd_);
      exit(-1);
  }
  set_ipport(serv_addr.ip(),serv_addr.port());

}

void Socket::listen(int nn) {
  if (::listen(fd_,128) != 0 )        // 在高并发的网络服务器中，第二个参数要大一些。
  {
    perror("listen() failed");
    close(fd_);
    exit(-1);
  }
}

int Socket::accept(InerAddress &client_addr) {
  sockaddr_in peer_addr;
  socklen_t len = sizeof(peer_addr);
  int clientfd = accept4(fd_,(sockaddr*)&peer_addr,&len,SOCK_NONBLOCK);
  //setnonblocking(clientfd);         // 客户端连接的fd必须设置为非阻塞的。
  client_addr.setaddr(peer_addr);
  return clientfd;
}

Socket::~Socket() {
  ::close(fd_);
}
Socket::Socket(int fd):fd_(fd) {}
std::string Socket::ip() const
{
  return ip_;
}
uint16_t Socket::port() const
{
  return port_;
}
void Socket::set_ipport(const std::string &ip,uint16_t port){
  ip_=ip;
  port_=port;
}
