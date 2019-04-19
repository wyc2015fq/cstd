# linux下socket连接下的心跳机制 - maopig的专栏 - CSDN博客
2016年03月19日 14:53:17[maopig](https://me.csdn.net/maopig)阅读数：5579
**1**，在长连接下，有可能很长一段时间都没有数据往来。
理论上说，这个连接是一直保持连接的，但是实际情况中，如果中间节点出现什么故障是难以知道的。
有的节点（防火墙）会自动把一定时间之内没有数据交互的连接给断掉。
在这个时候，就需要我们的心跳包了，用于维持长连接，保活
2，心跳包之所以叫心跳包是因为：它像心跳一样每隔固定时间发一次，以此来告诉服务器，这个客户端还活着。事实上这是为了保持长连接，至于这个包的内容，是没有什么特别规定的，不过一般都是很小的包，或者只包含包头的一个空包。心跳包主要也就是用于长连接的保活和断线处理。一般的应用下，判定时间在30-40秒比较不错。如果实在要求高，那就在6-9秒。
3，下面为封装好的心跳包函数，加入项目中参数设置一下即可
**[cpp]**[view plain](#)[copy](#)[print](#)[?](#)
- #include <netinet/tcp.h>  
- //参数解释 
- //fd:网络连接描述符
- //start:首次心跳侦测包发送之间的空闲时间  
- //interval:两次心跳侦测包之间的间隔时间 
- //count:探测次数，即将几次探测失败判定为TCP断开
- **int** set_tcp_keepAlive(**int** fd, **int** start, **int** interval, **int** count)     
- {     
- **int** keepAlive = 1;     
- **if** (fd < 0 || start < 0 || interval < 0 || count < 0) **return** -1;   //入口参数检查 ，编程的好习惯。
- //启用心跳机制，如果您想关闭，将keepAlive置零即可   
- **if**(setsockopt(fd,SOL_SOCKET,SO_KEEPALIVE,(**void***)&keepAlive,**sizeof**(keepAlive)) == -1)     
-     {     
-         perror("setsockopt");     
- **return** -1;     
-     }     
- //启用心跳机制开始到首次心跳侦测包发送之间的空闲时间   
- **if**(setsockopt(fd,SOL_TCP,TCP_KEEPIDLE,(**void** *)&start,**sizeof**(start)) == -1)     
-     {     
-         perror("setsockopt");     
- **return** -1;     
-     }     
- //两次心跳侦测包之间的间隔时间   
- **if**(setsockopt(fd,SOL_TCP,TCP_KEEPINTVL,(**void** *)&interval,**sizeof**(interval)) == -1)     
-     {     
-         perror("setsockopt");     
- **return** -1;     
-     }     
- //探测次数，即将几次探测失败判定为TCP断开   
- **if**(setsockopt(fd,SOL_TCP,TCP_KEEPCNT,(**void** *)&count,**sizeof**(count)) == -1)     
-     {     
-         perror("setsockopt");     
- **return** -1;     
-     }     
- **return** 0;     
- }   
```cpp
```
将想设置的参数传入该函数，设置成功返回0，否则返回-1。设置成功以后，可以将fd交给select去监听可读可写事件，如果select检测到fd可读且read返回错误(<0)，一般就能判定该fd对应的TCP连接已经异常断开，调用close函数将fd关闭即可。
TCP连接非正常断开的检测(KeepAlive探测)
此处的”非正常断开”指TCP连接不是以优雅的方式断开,如网线故障等物理链路的原因,还有突然主机断电等原因
有两种方法可以检测:1.TCP连接双方定时发握手消息 2.利用TCP协议栈中的KeepAlive探测
第二种方法简单可靠,只需对TCP连接两个Socket设定KeepAlive探测。
心跳机制只是做检查，并不能处理
从而得知连接已失效，客户端程序便有机会及时执行清除工作、提醒用户或重新连接。
