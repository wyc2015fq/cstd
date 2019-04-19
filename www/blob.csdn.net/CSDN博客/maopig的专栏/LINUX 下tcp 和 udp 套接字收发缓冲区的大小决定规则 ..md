# LINUX 下tcp 和 udp 套接字收发缓冲区的大小决定规则 . - maopig的专栏 - CSDN博客
2012年08月22日 23:38:07[maopig](https://me.csdn.net/maopig)阅读数：5233
const int udp_recvbufsize = 384 * 1024 ;
int result = ::setsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF,  
  (char*)&udp_recvbufsize, sizeof(int));
// 如果是由于你发送的速率较高而引起的,如500kbit/s, 那么设置大点的UDP缓冲区是比较有效的.
### [LINUX 下tcp 和 udp 套接字收发缓冲区的大小决定规则](http://blog.csdn.net/qiaoliang328/article/details/7580555)
分类： [网络编程](http://blog.csdn.net/qiaoliang328/article/category/592966)[LINUX](http://blog.csdn.net/qiaoliang328/article/category/569746)2012-05-18 16:02
413人阅读[评论](http://blog.csdn.net/qiaoliang328/article/details/7580555#comments)(0)[收藏]()[举报](http://blog.csdn.net/qiaoliang328/article/details/7580555#report)
1. tcp 收发缓冲区默认值 
[root@qljt core]# cat /proc/sys/net/ipv4/tcp_rmem  
4096    87380   4161536
87380  ：tcp接收缓冲区的默认值
[root@qljt core]# cat /proc/sys/net/ipv4/tcp_wmem 
4096    16384   4161536
16384  ： tcp 发送缓冲区的默认值
2. tcp 或udp收发缓冲区最大值
[root@qljt core]# cat /proc/sys/net/core/rmem_max 
131071
131071：tcp 或 udp 接收缓冲区最大可设置值的一半。
也就是说调用 setsockopt(s, SOL_SOCKET, SO_RCVBUF, &rcv_size, &optlen);  时rcv_size 如果超过 131071，那么
getsockopt(s, SOL_SOCKET, SO_RCVBUF, &rcv_size, &optlen); 去到的值就等于 131071 * 2 = 262142
[root@qljt core]# cat /proc/sys/net/core/wmem_max  
131071
131071：tcp 或 udp 发送缓冲区最大可设置值得一半。
跟上面同一个道理
3. udp收发缓冲区默认值
[root@qljt core]# cat /proc/sys/net/core/rmem_default   
111616：udp接收缓冲区的默认值
[root@qljt core]# cat /proc/sys/net/core/wmem_default 
111616
111616：udp发送缓冲区的默认值
4. tcp 或udp收发缓冲区最小值
tcp 或udp接收缓冲区的最小值为 256 bytes，由内核的宏决定；
tcp 或udp发送缓冲区的最小值为 2048 bytes，由内核的宏决定
