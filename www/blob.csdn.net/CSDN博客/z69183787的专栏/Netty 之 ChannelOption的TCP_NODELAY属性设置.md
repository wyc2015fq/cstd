# Netty 之 ChannelOption的TCP_NODELAY属性设置 - z69183787的专栏 - CSDN博客
2016年09月23日 11:51:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4337
在有些网络通信的场景下，要求低延迟，这样就需要我们设置一些TCP的链接属性：
在客户端我们需要这样设置：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51512606#)[copy](http://blog.csdn.net/linuu/article/details/51512606#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1698580/fork)
- bootstap.option(ChannelOption.TCP_NODELAY, true);  
在服务器端是在worker的Channel端设置属性，
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51512606#)[copy](http://blog.csdn.net/linuu/article/details/51512606#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1698580/fork)
- boot.childOption(ChannelOption.TCP_NODELAY, true);  
设置这样做好的好处就是禁用nagle算法
Nagle算法试图减少TCP包的数量和结构性开销, 将多个较小的包组合成较大的包进行发送.但这不是重点, 关键是这个算法受TCP延迟确认影响, 会导致相继两次向连接发送请求包,
读数据时会有一个最多达500毫秒的延时.
TCP/IP协议中，无论发送多少数据，总是要在数据前面加上协议头，同时，对方接收到数据，也需要发送ACK表示确认。为了尽可能的利用网络带宽，TCP总是希望尽可能的发送足够大的数据。（一个连接会设置MSS参数，因此，TCP/IP希望每次都能够以MSS尺寸的数据块来发送数据）。
Nagle算法就是为了尽可能发送大块数据，避免网络中充斥着许多小数据块。
详细可以参考：http://blog.csdn.net/historyasamirror/article/details/6423235
