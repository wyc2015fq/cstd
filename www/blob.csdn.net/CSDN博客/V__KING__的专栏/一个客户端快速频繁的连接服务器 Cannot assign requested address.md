# 一个客户端快速频繁的连接服务器:Cannot assign requested address - V__KING__的专栏 - CSDN博客





2018年06月29日 18:36:19[v__king__](https://me.csdn.net/V__KING__)阅读数：758








背景机制： 

1. 一个tcp服务器，端口1234；服务器限定只能维持一个tcp连接。该tcp服务器是原先已有的。 

2. 一个web服务器，使用tcp客户端连接tcp server的1234端口。一个post请求对应一次[tcp client连接tcp server:1234]，完成post请求后，断开该tcp连接。 

问题现象：
> 
当其中一个web页面中有多个ajax的post请求时，tcp client连接不上tcp server，并出现错误。


原因： 

参考[https://www.jianshu.com/p/51a953b789a4](https://www.jianshu.com/p/51a953b789a4)

> - 
大致上是由于客户端频繁的连服务器，由于每次连接都在很短的时间内结束，导致很多的TIME_WAIT，**以至于用光了可用的端 口号**，所以新的连接没办法绑定端口，即“Cannot assign requested address”。是客户端的问题不是服务器端的问题。通过netstat，的确看到很多TIME_WAIT状态的连接。

- 
client端频繁建立连接，而端口释放较慢，导致建立新连接时无可用端口。



解决方法1(没啥效果)：

执行命令修改如下2个内核参数 （需要root权限） 

```
sysctl -w net.ipv4.tcp_timestamps=1  开启对于TCP时间戳的支持,若该项设置为0，则下面一项设置不起作用
sysctl -w net.ipv4.tcp_tw_recycle=1  表示开启TCP连接中TIME-WAIT sockets的快速回收
```

解决方法2： 

这里我用的是该方法，放慢速度去连接服务器。当有多个ajax的post需要同时请求时，就一个一个的去请求。




