#include"InerAddress.h"

/*
class InerAddress
{
private:
  sockaddr_in addr_;
public:
  InerAddress(const std::string &ip,uint16_t port);
  InerAddress(const sockaddr_in addr):addr_(addr){}
  ~InerAddress();
  const char *ip() const;
  uint16_t port() const;
  const sockaddr *addr()const;
};
*/
InerAddress::  InerAddress(const std::string &ip,uint16_t port){
    addr_.sin_family = AF_INET;                              // IPv4网络协议的套接字类型。
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());      // 服务端用于监听的ip地址。
    addr_.sin_port = htons(port);
}
InerAddress::InerAddress(const sockaddr_in addr):addr_(addr){

}
InerAddress::InerAddress(){

}
InerAddress::~InerAddress(){

}
const char *InerAddress::ip()const{
  return inet_ntoa(addr_.sin_addr);
}
uint16_t InerAddress::port()const{
  return ntohs(addr_.sin_port);
}
const sockaddr *InerAddress::addr()const{
  return (sockaddr*)&addr_;
}
void InerAddress::setaddr(sockaddr_in client_addr){
  addr_=client_addr;
}
