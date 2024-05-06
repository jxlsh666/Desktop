#include"Epoll.h"
/*
void Epoll::add_fd(int fd,uint32_t op){
  struct epoll_event ev;              // 声明事件的数据结构。
  ev.data.fd=fd;                 // 指定事件的自定义数据，会随着epoll_wait()返回的事件一并返回。
  ev.events=op;                // 让epoll监/视listenfd的读事件，采用水平触发。

  if(epoll_ctl(epollfd_,EPOLL_CTL_ADD,fd,&ev)){
    printf("epoll_ctl failed(%d).\n",errno);
    exit(-1);
  }     // 把需要监视的listenfd和它的事件加入epollfd中。

}*/
void Epoll::update_channel(Channel *ch){
  epoll_event ev;
  ev.data.ptr=ch;
  ev.events=ch->events();
  if(ch->in_poll()){
    if(epoll_ctl(epollfd_,EPOLL_CTL_MOD,ch->fd(),&ev)==-1){//update
      perror("epoll_ctl() failed\n");
      exit(-1);
    }
  }else{
    if(epoll_ctl(epollfd_,EPOLL_CTL_ADD,ch->fd(),&ev)==-1){//add
      perror("epoll_ctl() failed\n");
      exit(-1);
    }
    ch->set_in_epoll(true);
  }
}
std::vector<Channel*> Epoll:: loop(int timeout){
  std::vector<Channel*> channels;
  bzero(events_,sizeof(events_));
  int infds=epoll_wait(epollfd_,events_,MaxEvents,timeout);       // 等待监视的fd有事件发生。

  // 返回失败。
  if (infds < 0)
  {
      perror("epoll_wait() failed");
      exit(-1);
  }

  // 超时。
  if (infds == 0)
  {
      printf("epoll_wait() timeout.\n");
      return channels;
  }

  // 如果infds>0，表示有事件发生的fd的数量。
  for (int ii=0;ii<infds;ii++)       // 遍历epoll返回的数组evs。
  {
    Channel* ch=(Channel*)events_[ii].data.ptr;
    ch->set_revents(events_[ii].events);
    channels.push_back(ch);
  }
  return channels;
}

Epoll::Epoll(){

  if((epollfd_=epoll_create(1))==-1){
    perror("epollfd_ create error");
    exit(-1);
  }
}
Epoll::~Epoll(){
  close(epollfd_);
}
void Epoll::remove_channel(Channel *ch){
  if(ch->in_poll())
  {
    printf("remove_channel");
    if(epoll_ctl(epollfd_,EPOLL_CTL_DEL,ch->fd(),0)==-1)//delete
    {
      perror("epoll_ctl() failed\n");
      exit(-1);
    }
  }
}
