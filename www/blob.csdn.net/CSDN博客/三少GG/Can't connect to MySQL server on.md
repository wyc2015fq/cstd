# Can't connect to MySQL server on - 三少GG - CSDN博客
2013年11月21日 14:14:46[三少GG](https://me.csdn.net/scut1135)阅读数：1002
**coredump:**
![](https://img-blog.csdn.net/20131121141203671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGt1ZWVjc2Vy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**打印日志发现：mysql_real_connect()返回如下的错误信息（c++客户端）**
**Can't connect to MySQL server on 'xx.xx.xx.xx' (99)**
问题分析：
客户端频繁的连服务器，由于每次连接都在很短的时间内结束，导致很多的TIME_WAIT，以至于用光了可用的端 口号，所以新的连接没办法绑定端口，即“Cannot assign requested address”。是客户端的问题不是服务器端的问题。通过netstat，的确看到很多TIME_WAIT状态的连接。
可见问题不在于server端，**而是client端频繁建立连接，而端口释放较慢，导致建立新连接时无可用端口**。
解决方法：
执行命令修改如下2个内核参数 （需要root权限） 
**sysctl -w net.ipv4.tcp_timestamps=1 **
 //开启对于TCP时间戳的支持,若该项设置为0，则下面一项设置不起作用
**sysctl -w net.ipv4.tcp_tw_recycle=1  **//表示开启TCP连接中TIME-WAIT sockets的快速回收
