
# 关于非阻塞的recv的时候返回的处理 - 高科的专栏 - CSDN博客

2013年12月03日 16:42:09[高科](https://me.csdn.net/pbymw8iwm)阅读数：25208


## 注意recv（）如果读到数据为0，那么就表示文件结束了，如果在读的过程中遇到了中断那么会返回-1，同时置errno为EINTR。
因此判断recv的条件：
如果read返回<=0
如果==0
表示文件结束， 处理
如果<0 && errno==EINTR
表示中断，处理
否则，出错
我们举个例子：
int safe_tcp_recv (int sockfd, void *buf, int bufsize)
{
int cur_len;
recv_again:
cur_len = recv (sockfd, buf, bufsize, 0);
//closed by client
if (cur_len == 0)
{
TRACE_LOG ("connection closed by peer, fd=%d", sockfd);
return 0;
}
else if (cur_len == -1)
{
if (errno == EINTR)
goto recv_again;
else
ERROR_LOG ("recv error, fd=%d, errno=%d %m", sockfd, errno);
}
return cur_len;
}
或者
int safe_tcp_recv_n (int sockfd, void *buf, int total)
{
int recv_bytes, cur_len;
for (recv_bytes = 0; recv_bytes < total; recv_bytes += cur_len)
{
cur_len = recv (sockfd, buf + recv_bytes, total - recv_bytes, 0);
//closed by client
if (cur_len == 0) //正常关闭
{
TRACE_LOG ("connection closed by peer, fd=%d", sockfd);
return -1;
}
else if (cur_len == -1)
{
if (errno == EINTR)//还需要在读
cur_len = 0;
else if (errno == EAGAIN)
{
TRACE_LOG ("recv %d bytes from fd=%d", recv_bytes, sockfd);
return recv_bytes;
}
else
{
ERROR_RETURN (("recv tcp packet error, fd=%d, %m", sockfd), -1);
}
}
}
return recv_bytes;
}
但是write（）如果写入的数据为0，那么就表示出错，也就是无法写入了，而如果在写的过程中遇到了中断，那么write（）会返回-1，同时置errno为EINTR。
因此判断write是否成功时，条件是write返回的结果是否<=0
int safe_tcp_send_n (int sockfd, const void *buf, int total)
{
int send_bytes, cur_len;
for (send_bytes = 0; send_bytes < total; send_bytes += cur_len)
{
cur_len = send (sockfd, buf + send_bytes, total - send_bytes, 0);
//closed by client
if (cur_len == 0)
{
TRACE_LOG ("send tcp packet error, fd=%d, %m", sockfd);
return -1;
}
else if (cur_len == -1)
{
if (errno == EINTR)
cur_len = 0;
else if (errno == EAGAIN)
{
return send_bytes;
}
else
{
TRACE_LOG ("send tcp packet error, fd=%d, %m", sockfd);
return -1;
}
}
}
//TRACE_LOG ("send: fd=%d, len=%d", sockfd, send_bytes);
return send_bytes;
}
首先我们看看recv的返回值：
EAGAIN、EWOULDBLOCK、EINTR与非阻塞 长连接
EWOULDBLOCK用于非阻塞模式，不需要重新读或者写
EINTR指操作被中断唤醒，需要重新读/写
在Linux环境下开发经常会碰到很多错误(设置errno)，其中EAGAIN是其中比较常见的一个错误(比如用在非阻塞操作中)。
从字面上来看，是提示再试一次。这个错误经常出现在当应用程序进行一些非阻塞(non-blocking)操作(对文件或socket)的时候。例如，以 O_NONBLOCK的标志打开文件/socket/FIFO，如果你连续做read操作而没有数据可读。此时程序不会阻塞起来等待数据准备就绪返 回，read函数会返回一个错误EAGAIN，提示你的应用程序现在没有数据可读请稍后再试。重新读数据，其实这个也就是说明了一点，网络报还没有发完结束呢，还需要继续读，除非返回的是大于0的值，就代表的是读完了，返回正常读到的网络报的长度。
又例如，当一个系统调用(比如fork)因为没有足够的资源(比如虚拟内存)而执行失败，返回EAGAIN提示其再调用一次(也许下次就能成功)。
Linux - 非阻塞socket编程处理EAGAIN错误
在linux进行非阻塞的socket接收数据时经常出现Resource temporarily unavailable，errno代码为11(EAGAIN)，这是什么意思？
这表明你在非阻塞模式下调用了阻塞操作，在该操作没有完成就返回这个错误，这个错误不会破坏socket的同步，不用管它，下次循环接着recv就可以。 对非阻塞socket而言，EAGAIN不是一种错误。在VxWorks和Windows上，EAGAIN的名字叫做EWOULDBLOCK。
另外，如果出现EINTR即errno为4，错误描述Interrupted system call，操作也应该继续。
最后，如果recv的返回值为0，那表明连接已经断开，我们的接收操作也应该结


