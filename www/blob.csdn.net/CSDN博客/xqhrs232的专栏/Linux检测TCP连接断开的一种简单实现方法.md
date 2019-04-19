# Linux检测TCP连接断开的一种简单实现方法 - xqhrs232的专栏 - CSDN博客
2017年01月05日 21:48:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2148
原文地址::[http://blog.csdn.net/wtewrw/article/details/6547380](http://blog.csdn.net/wtewrw/article/details/6547380)
相关文章
1、[TCP连接状态详解](http://blog.csdn.net/wtewrw/article/details/6556418)----[http://blog.csdn.net/wtewrw/article/details/6556418](http://blog.csdn.net/wtewrw/article/details/6556418)
2、How to determine terminated TCP connection in Linux?----[http://compgroups.net/comp.arch.embedded/how-to-determine-terminated-tcp-connection/1287003](http://compgroups.net/comp.arch.embedded/how-to-determine-terminated-tcp-connection/1287003)
3、[Linux网络编程之使用TCP传输文件](http://blog.csdn.net/gqb_driver/article/details/8976263)----[http://blog.csdn.net/wangyin159/article/details/47043269](http://blog.csdn.net/wangyin159/article/details/47043269)
Linux检测TCP连接断开的一种简单实现方法
一般可以采取业务层的心跳检测，超时没有收到心跳包，就认为是对端已经断开，还有些更高级的方法比如KeepAlive，但都不外乎互相定时发点校验信息。
我要说的解法：
第一步，select函数的返回值判断。在年ucLinux,EM8511平台下跑，select没有数据时返回的是-1，有数据返回的就是大于0的整数，好像还没有碰到过0的时候。 千万不要以为返回-1就是错误。          
*小提示：select函数调用之后会将清空delay中的值（清成0），如果是反复调用select并希望有一定延时的话，一定要记得每次调用前都要指定延时参数。*
第二步，检测到select是1，说明你的对端有可能是关闭了，千万不要高高兴兴不顾一切地就去读数据。实际上这个时候由于断开之后TCP/IP本身的一些握手信号，select会认为socket中有数据。这个使用如果使用read去读，将会返回0。这时select和read的结果明显是不一致的，我们认为是对端端口断开。
用这个方法检测，未必是完全之策，但是在我的程序上用起来还是很爽的。
