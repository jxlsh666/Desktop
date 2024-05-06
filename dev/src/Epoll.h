#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <sys/epoll.h>
#include <vector>
#include <unistd.h>
#include "Channel.h"

class Channel;
class Epoll
{
private:
static const int MaxEvents=100;
  int epollfd_=-1;//epoll句柄
  epoll_event events_[MaxEvents];
public:
  Epoll();
  ~Epoll();
//  void add_fd(int fd,uint32_t op);//添加监视事件
  void update_channel(Channel *ch);
  void remove_channel(Channel *ch);
//  std::vector<epoll_event> loop(int timeout=-1);
  std::vector<Channel*> loop(int timeout=-1);

};

