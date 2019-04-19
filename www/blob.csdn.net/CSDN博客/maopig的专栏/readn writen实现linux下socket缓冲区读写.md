# readn writen实现linux下socket缓冲区读写 - maopig的专栏 - CSDN博客
2011年09月11日 01:02:45[maopig](https://me.csdn.net/maopig)阅读数：914标签：[socket																[linux																[io](https://so.csdn.net/so/search/s.do?q=io&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
个人分类：[网络																[Linux](https://blog.csdn.net/maopig/article/category/850267)](https://blog.csdn.net/maopig/article/category/869532)
socket上的read write 操作不同与一般的文件IO操作，socket上的用read write读写的字节数可能比要求的少,但这并不是错误，原因是socket的缓冲区可能已经达到了极限。此时所需要的就是再次调用read write 以写入或输出剩余的字符。这种情况在socket中很常见，但在写字节流socket时只能在socket非堵塞的情况下才会出现，然而为预防实现万一返回不足的字符数值，我们总是调用writen和readn函数，而不是read和write.
下面就是readn、writen函数源码：
ssize_t      /* Read "n" bytes from a descriptor. */
readn(int fd, void *vptr, size_t n)
{
 size_t nleft;
 ssize_t nread;
 char *ptr;
 ptr = vptr;
 nleft = n;
 while (nleft > 0) {
  if ( (nread = read(fd, ptr, nleft)) < 0) {
   if (errno == EINTR)
    nread = 0;  /* and call read() again */
   else
    return(-1);
  } else if (nread == 0)
   break;    /* EOF */
  nleft -= nread;
  ptr   += nread;
 }
 return(n - nleft);  /* return >= 0 */
}
ssize_t      /* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n)
{
 size_t  nleft;
 ssize_t  nwritten;
 const char *ptr;
 ptr = vptr;
 nleft = n;
 while (nleft > 0) {
  if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
   if (nwritten < 0 && errno == EINTR)
    nwritten = 0;  /* and call write() again */
   else
    return(-1);   /* error */
  }
  nleft -= nwritten;
  ptr   += nwritten;
 }
 return(n);
}
