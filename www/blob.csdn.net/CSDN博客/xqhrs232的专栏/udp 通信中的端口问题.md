# udp 通信中的端口问题 - xqhrs232的专栏 - CSDN博客
2018年08月15日 14:54:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：709
原文地址::[http://blog.sina.com.cn/s/blog_7ffbb788010117hd.html](http://blog.sina.com.cn/s/blog_7ffbb788010117hd.html)
相关文章
1、UDP通信，如何指定接收端口跟发送端口 ----[https://bbs.csdn.net/topics/390241696](https://bbs.csdn.net/topics/390241696)
最近写了个天气预报的小程序，基于c/s模式，client向server发送请求，server定时向所有client发送天气更新，两者之间采用无连接的udp通信。以前几乎没有碰过socket网络编程，我也是边学边做的，下面是我在此过程中遇到的一些问题，希望给刚开始接触网络编程的朋友一些借鉴作用，少走弯路。
主要用到两个函数
recvfrom，原型如下：
函数原型:int recvfrom(int sockfd,void *buf,int len,unsigned int flags, struct sockaddr *from,int *fromlen);
sendto，原型如下：
int SendTo(SOCKET s; char * buf, int len, int socketFlags,struct sockaddr  *to ; int tolen);
**错误做法：**
server端新建一个socket并且bind到一个固定端口，循环调用recvfrom函数来监听客户端的请求,当接受到某个client的第一个请求后，记下from参数中的的ip和端口信息（记为x），并发送相应的信息给client。
定时更新时利用x中存储的客户端地址和端口信息发送更新
这种做法，client是收不到server发送的定时更新信息的，因为端口不对。
**错误原因：**
对于client，socket没有bind到某个端口，sendto在每次发送信息时候，那么，每次系统会随机分派一个可用端口给其发送，发送后如果调用recvfrom函数，则recvfrom是从该端口接收server发送的返回消息，但是下次就不能在该端口recvfrom到消息了，即端口是一次性的，所以server发送更新的时候，client是接收不到的。
**正确做法：**
client在第一次连接到server的时候，需在相应的数据包中携带自己的监听端口信息，并且监听使用的socket需要bind到该端口上面。server在第一次收到client的消息时是记下数据包的端口而不是from参数中的端口。这样server每次发送更新时向这个端口发送消息，client就能收到了。
