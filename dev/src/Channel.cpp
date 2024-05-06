#include"Channel.h"

/*
class Channel
{
private:
  int fd_=-1;         //Channel唯一fd
  Epoll*ep_=nullptr;  //对应的红黑树，多对一Epoll
  bool inepoll_=false;// 是否在树上
  uint32_t events_=0; //监视事件
  uint32_t revents_=0; //已发生事件
};
Channel::
*/
Channel::Channel(EventLoop*loop,int fd){
  loop_=loop;
  fd_=fd;
}
Channel::~Channel(){

}
int Channel:: fd(){
  return fd_;
}
void Channel::use_et(){
  events_|=EPOLLET;
}          //边缘触发

void Channel::set_in_epoll(bool inepoll){
  inepoll_=inepoll;
}
void Channel::set_revents(uint32_t ev){
  revents_=ev;
}
bool Channel::in_poll(){
  return inepoll_;
}
uint32_t Channel::events(){
  return events_;
}
uint32_t Channel::revents(){
  return revents_;
}

void Channel::handle_event(){
  if (revents_ & EPOLLRDHUP)                     // 对方已关闭，有些系统检测不到，可以使用EPOLLIN，recv()返回0。
  {
    close_callback_();
    remove();
    /*
    printf("1client(eventfd=%d) disconnected.\n",fd_);
    close(fd_);            // 关闭客户端的fd。
    */
  }
  else if (revents_ & (EPOLLIN|EPOLLPRI))   // 接收缓冲区中有数据可以读。
  {
    read_callback_();
  }
  else if (revents_ & EPOLLOUT)                  // 有数据需要写，暂时没有代码，以后再说。
  {
    write_callback_();
  }
  else                                                                    // 其它事件，都视为错误。
  {
    remove();
    error_callback_();
    /*
      printf("3client(eventfd=%d) error.\n",fd_);
      close(fd_);            // 关闭客户端的fd。
    */
  }
}

void Channel::set_read_callback(std::function<void()> fn){
  read_callback_=fn;
}
void Channel::set_close_callback(std::function<void()> fn){
  close_callback_=fn;
}
void Channel::set_error_callback(std::function<void()> fn){
  error_callback_=fn;
}
void Channel::set_write_callback(std::function<void()> fn){
  write_callback_=fn;
}
void Channel::enable_reading(){
  events_|=EPOLLIN;
  loop_->update_channel(this);
}  //监视读事件
void Channel::disable_reading()  //取消监视读事件
{
  events_&=~EPOLLIN;
  loop_->update_channel(this);
}
void Channel::enable_writing()  //监视写事件
{
  events_|=EPOLLOUT;
  loop_->update_channel(this);
}
void Channel::disable_writing()  //取消监视读事件
{
  events_&=~EPOLLOUT;
  loop_->update_channel(this);
}
void Channel::disable_all()     //取消事件
{
  events_=0;
  loop_->update_channel(this);
}
void Channel::remove()
{
  disable_all();
  loop_->remove_channel(this);
}
