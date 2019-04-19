# linux socket 缓冲区默认大小 - xqhrs232的专栏 - CSDN博客
2016年06月29日 17:09:40[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：727
原文地址::[http://blog.csdn.net/herecles/article/details/8146017](http://blog.csdn.net/herecles/article/details/8146017)
相关文章
1、linux下getsockopt和setsockopt详解及测试----[http://www.tuicool.com/articles/RZvUra](http://www.tuicool.com/articles/RZvUra)
2、[linux
 send与recv函数详解](http://www.cnblogs.com/blankqdb/archive/2012/08/30/2663859.html)----[http://www.cnblogs.com/blankqdb/archive/2012/08/30/2663859.html](http://www.cnblogs.com/blankqdb/archive/2012/08/30/2663859.html)
1. tcp 收发缓冲区默认值
[root@ www.linuxidc.com]# cat /proc/sys/net/ipv4/tcp_rmem  
4096    87380   4161536
87380  ：tcp接收缓冲区的默认值
[root@ www.linuxidc.com]# cat /proc/sys/net/ipv4/tcp_wmem 
4096    16384   4161536
16384  ： tcp 发送缓冲区的默认值
2. tcp 或udp收发缓冲区最大值
[root@ www.linuxidc.com]# cat /proc/sys/net/core/rmem_max 
131071
131071：tcp 或 udp 接收缓冲区最大可设置值的一半。
也就是说调用 setsockopt(s, SOL_SOCKET, SO_RCVBUF, &rcv_size, &optlen);  时rcv_size 如果超过 131071，那么
getsockopt(s, SOL_SOCKET, SO_RCVBUF, &rcv_size, &optlen); 去到的值就等于 131071 * 2 = 262142
[root@ www.linuxidc.com]# cat /proc/sys/net/core/wmem_max  
131071
131071：tcp 或 udp 发送缓冲区最大可设置值得一半。
跟上面同一个道理
3. udp收发缓冲区默认值
[root@ www.linuxidc.com]# cat /proc/sys/net/core/rmem_default   
111616：udp接收缓冲区的默认值
[root@ www.linuxidc.com]# cat /proc/sys/net/core/wmem_default 
111616
111616：udp发送缓冲区的默认值
4. tcp 或udp收发缓冲区最小值
tcp 或udp接收缓冲区的最小值为 256 bytes，由内核的宏决定；
tcp 或udp发送缓冲区的最小值为 2048 bytes，由内核的宏决定
本篇文章来源于 Linux公社网站(www.linuxidc.com)  原文链接：http://www.linuxidc.com/Linux/2012-08/68874.htm
