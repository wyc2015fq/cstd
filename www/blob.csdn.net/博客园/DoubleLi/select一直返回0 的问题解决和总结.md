# "select一直返回0"的问题解决和总结 - DoubleLi - 博客园






# 场景：一个简单的TCP 服务器,以实现UPNP的事件体系结构

我在linux平台下，创建一个TCP套接字，绑定到49156端口，向UPNP SERVER发一个subscribe订阅请求，超时时间设置为5minutes.

然后开启一个Thread_Main主接收线程。该线程完成以下工作：

（1）调用select监听是否有数据可读,设置4s的超时；

（2）如果select返回值正常（>0），则调用accept,接收客户端请求；

（3）调用recv接收客户端数据；

（4）解析收到的TCP裸数据；

其软件架构如下图所示：：


![](http://img.blog.csdn.net/20131224165217015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWNzNzEz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

# 问题：数据接收开始是正常的，过一阵子就接收不到数据了，select总是返回0

初步怀疑：

（1）由于发送的subscribe订阅请求是有超时限制的，因此必须在超时前向upnp server发送续订请求。

代码初步改动如下：

![](http://img.blog.csdn.net/20131224165622281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWNzNzEz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

按这种思路，添加续订请求后，问题依然存在，百思不得其解。

通过wireshark抓包分析，发现这种情况下，我的机器是收到了upnp server的notify消息的。

那为什么我的TCP程序却解析不到呢？

通过添加打印发现，异常情况下， select函数始终返回0.也就是说，我的TCP服务器程序始终认为没有数据可读或超。这是一个很奇怪的现象。

因此判定这段TCP服务器程序存在bug。



# 问题解决：每次调用select之前，调用FD_ZERO清空可读文件句柄集，并调用FD_SET把TCP套接字添加到该fd_set类型的集合中。



代码对比：

![](http://img.blog.csdn.net/20131224170106203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWNzNzEz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)











