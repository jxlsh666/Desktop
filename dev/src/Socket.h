#pragma once


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <sys/epoll.h>
#include <netinet/tcp.h>
#include"InerAddress.h"
int create_non_blocking();
class Socket
{
private:
  const int fd_;
  std::string ip_;
  uint16_t port_;
public:
  Socket(int fd);
  int fd();
  std::string ip() const;
  uint16_t port() const;
  void set_reuse_addr(bool on);
  void set_reuse_port(bool on);
  void set_tcp_nodelay(bool on);
  void set_keep_alive(bool on);
  void set_ipport(const std::string &ip,uint16_t port);
  void bind(const InerAddress& serv_addr);
  void listen(int nn=128);
  int accept(InerAddress& client_addr);
  ~Socket();
};
