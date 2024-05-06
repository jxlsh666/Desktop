/*
 * 程序名：tcpepoll.cpp，此程序用于演示采用epoll模型实现网络通讯的服务端。
 * 作者：吴从周
*/

#include <stdio.h>
/*
#include "InerAddress.h"
#include"Epoll.h"
#include"Socket.h"
#include"Channel.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <sys/epoll.h>
#include <netinet/tcp.h>      // TCP_NODELAY需要包含这个头文件。
#include"EventLoop.h"
*/
#include"EchoServer.h"

int main(int argc,char *argv[])
{
  if (argc != 3)
  {
      printf("usage: ./tcpepoll ip port\n");
      printf("example: ./tcpepoll 127.0.0.1 5085\n\n");
      return -1;
  }
  EchoServer echo_server(argv[1],atoi(argv[2]),3,0);
  echo_server.Start();
  return 0;
}
