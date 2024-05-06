#include"Connection.h"

Connection:: Connection(EventLoop *loop,std::unique_ptr<Socket> client_sock)
:client_channel_(new Channel(loop_,client_sock->fd()))
{
  // 为新客户端连接准备读事件，并添加到epoll中
  loop_=loop;
  client_sock_=std::move(client_sock);
  disconnect_=false;

  //客户端只会接收消息，所以绑定on_message
  client_channel_->set_read_callback(std::bind(&Connection::on_message,this));
  client_channel_->set_close_callback(std::bind(&Connection::close_callback,this));
  client_channel_->set_error_callback(std::bind(&Connection::error_callback,this));
  client_channel_->set_write_callback(std::bind(&Connection::write_callback,this));

  client_channel_->use_et();
  client_channel_->enable_reading();
}
Connection:: ~Connection(){
  //delete client_channel_;
  //delete client_sock_;
}
std::string Connection::ip() const
{
  return client_sock_->ip();
}
uint16_t Connection::port() const
{
  return client_sock_->port();
}
int Connection::fd()const
{
  return client_sock_->fd();
}
void Connection::close_callback()
{
  disconnect_=true;
  close_callback_(shared_from_this());           // 关闭客户端的fd。
}
void Connection::error_callback()
{
  disconnect_=true;
  error_callback_(shared_from_this());
}
void Connection::set_close_callback(std::function<void(spConnection)> fn){
  close_callback_=fn;
}
void Connection::set_error_callback(std::function<void(spConnection)> fn){
  error_callback_=fn;
}
// 设置处理报文的回调函数。
void Connection::setonmessagecallback(std::function<void(spConnection,std::string&)> fn)
{
    onmessagecallback_=fn;       // 回调TcpServer::onmessage()。
}
void Connection::on_message()//处理对端消息
{
  char buffer[1024];
  while (true)             // 由于使用非阻塞IO，一次读取buffer大小数据，直到全部的数据读取完毕。
  {
    bzero(&buffer, sizeof(buffer));
    ssize_t nread = read(fd(), buffer, sizeof(buffer));    // 这行代码用了read()，也可以用recv()，一样的，不要纠结。
    if (nread > 0)      // 成功的读取到了数据。
    {
      input_buffer_.append(buffer,nread);
    }
    else if (nread == -1 && errno == EINTR) // 读取数据的时候被信号中断，继续读取。
    {
      continue;
    }
    else if (nread == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) // 全部的数据已读取完毕。
    {
      while (true)
      {
        //////////////////////////////////////////////////////////////
        // 可以把以下代码封装在Buffer类中，还可以支持固定长度、指定报文长度和分隔符等多种格式。
        int len;
        memcpy(&len,input_buffer_.data(),4);     // 从inputbuffer中获取报文头部。
        // 如果inputbuffer中的数据量小于报文头部，说明inputbuffer中的报文内容不完整。
        if (input_buffer_.size()<len+4) break;

        std::string message(input_buffer_.data()+4,len);   // 从inputbuffer中获取一个报文。
        input_buffer_.erase(0,len+4);                                 // 从inputbuffer中删除刚才已获取的报文。
        //////////////////////////////////////////////////////////////

        printf("message (eventfd=%d):%s\n",fd(),message.c_str());
        onmessagecallback_(shared_from_this(),message);       // 回调TcpServer::onmessage()。
      }
      break;
    }
    else if (nread == 0)  // 客户端连接已断开。
    {
      client_channel_->remove();
      close_callback();
      break;
    }
  }
}

void Connection::send(const char *data,size_t size){
  if(disconnect_==true){ printf("连接已经断开\n");return;}
  output_buffer_.append_head(data,size);
  client_channel_->enable_writing();

}
void Connection::write_callback()//处理写事件
{
  int writen=::send(fd(),output_buffer_.data(),output_buffer_.size(),0);
  if(writen>0){
    output_buffer_.erase(0,writen);
  }
  if(output_buffer_.size()==0){
    client_channel_->disable_writing();
    send_complete_callback_(shared_from_this());
  }
}
void Connection::set_complete_callback(std::function<void(spConnection)> fn){
  send_complete_callback_=fn;
}
