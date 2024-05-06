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
class InerAddress
{
private:
  sockaddr_in addr_;
public:
  InerAddress();
  InerAddress(const std::string &ip,uint16_t port);
  InerAddress(const sockaddr_in addr);
  ~InerAddress();
  const char *ip() const;
  uint16_t port() const;
  const sockaddr *addr()const;
  void setaddr(sockaddr_in client_addr);
};

