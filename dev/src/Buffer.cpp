#include"Buffer.h"
/*
  void append(const char *data,size_t size);
  size_t size();
  const char *data();
  void clear();
};


*/
Buffer::Buffer(/* args */)
{

}

Buffer::~Buffer()
{

}
void Buffer::append(const char *data,size_t size){
  buf_.append(data,size);
}
size_t Buffer::size()
{
  return buf_.size();
}
const char *Buffer::data()
{
  return buf_.data();
}
void Buffer::clear()
{
  buf_.clear();
}
// 从buf_的pos开始，删除nn个字节，pos从0开始。
void Buffer::erase(size_t pos,size_t nn)
{
    buf_.erase(pos,nn);
}
void Buffer::append_head(const char *data,size_t size){
  buf_.append((char *)&size,4);
  buf_.append(data,size);
}
