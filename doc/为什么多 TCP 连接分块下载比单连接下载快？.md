# 为什么多 TCP 连接分块下载比单连接下载快？

我观察到，客户端机器从单一服务器使用 HTTP 下载一个文件：
1. 单连接下载，速度没有达到客户端网络的最大带宽；
2. 多连接同时下载，传输速度有极大的提高，带宽被占满。

假设如下前提：
1. 服务器是单一的，没有使用提供相同文件的其它服务器，也没有使用同域名的其它服务器；
2. 服务器不对单个连接限速。

那么，是什么导致多连接下载的速度大为提高呢？换一种说法，是什么原因导致单一 TCP 连接没有尽可能地利用带宽呢？
是因为不同的 TCP 连接使用了不同的链路吗？可是传输层不应该影响网络层的吧？
是因为 TCP 本身的特性吗？那又是怎样的特性导致了这种结果呢？

测试结果：
1. 单连接下载：wget --header='Host: python.org' http://82.94.164.162/ftp/python/3.4.0/Python-3.4.0a3.tar.xz 138 KB/s
2. 多连接下载：aria2c -k 1M -x 16 -s 16 --header='Host: python.org' http://82.94.164.162/ftp/python/3.4.0/Python-3.4.0a3.tar.xz 414KiB/s
3. 国外服务器单连接下载： 2.26 MB/s

补充：文件是下载到内存的（tmpfs），因此避开了并发磁盘 I/O 带来的影响。



作者：知乎用户

链接：https://www.zhihu.com/question/21813579/answer/19402704

来源：知乎

著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

TCP特性使得每个TCP连接可以得到均等的带宽。在多用户环境下，一个用户拥有越多TCP连接，获得的带宽越大。

具体来说：

这个涉及到了TCP的拥塞控制。

我们先看一下单TCP连接的拥塞控制。

这是一个TCP连接的发送窗口。

![img](https://pic3.zhimg.com/50/ac38d18e3cd9c7941bbd753fe37bbbba_hd.jpg)![img](https://pic3.zhimg.com/80/ac38d18e3cd9c7941bbd753fe37bbbba_hd.jpg)

绿色部分为发送者已发送，且接收者已确认（ACKed）。

黄色部分为发送者已发送，但接收者尚未确认（"in-flight"）。

蓝色部分为可用但尚未发送。

灰色部分为不可用。



所以在RTT（round-trip time，来回通讯延迟）不变的情况下，cwnd这个变量基本决定传输速率。

![img](https://pic1.zhimg.com/50/382de3cc7379025d169f7e3264368252_hd.jpg)![img](https://pic1.zhimg.com/80/382de3cc7379025d169f7e3264368252_hd.jpg)

发送者总会试图找到不丢包情况下的最大速率。按照TCP协议，在传输开始之后，每接收到一个确认（ACK)就会把cwnd这个变量增大一倍。所以TCP连接开始之后应该是这个样子。

![img](https://pic1.zhimg.com/50/d6eceab08d6e7e33b62f918502c2558d_hd.jpg)![img](https://pic1.zhimg.com/80/d6eceab08d6e7e33b62f918502c2558d_hd.jpg)

刚开始的时候传输速率应该是指数被增长的，直到丢包发生。丢包会有两种情况：

1.当接收者发送给发送者的ACK丢失了，这时会触发超时（timeout)。

2.当发送者发送给接收者的数据包丢失了，发送者会收到接收者发来的重复ACK，如果发送者收到了3个重复的ACK，也会认为发生了丢包。



具体对这两种情况采取的措施略有不同，但粗略来说，变量cwnd会被减半，也就是说传输速率减半。然后cwnd会再次增大，直到下次丢包发生。所以忽略最开始，TCP的吞吐量应该是这样。

![img](https://pic4.zhimg.com/50/931df6c47759a69e63cfd37c82594b21_hd.jpg)![img](https://pic4.zhimg.com/80/931df6c47759a69e63cfd37c82594b21_hd.jpg)

好，那么现在我们来看多TCP连接的拥塞控制。

我们假设有两条同样的TCP连接。在他们的连接中间有一个共用的瓶颈路由器，带宽为R。

![img](https://pic2.zhimg.com/50/91b2492e537591557e5009f814aefef2_hd.jpg)![img](https://pic2.zhimg.com/80/91b2492e537591557e5009f814aefef2_hd.jpg)

假设这两条连接都需要传输足够大量的数据，那么不论他们谁先开始传输，最后一定会均分带宽。

![img](https://pic1.zhimg.com/50/6dab723d3bbdffb03c748a1b69bba2df_hd.jpg)![img](https://pic1.zhimg.com/80/6dab723d3bbdffb03c748a1b69bba2df_hd.jpg)

因为如果总传输速率低于R的时候就会不断增大传输速率，某个连接在增大传输速率的时候发生丢包就会减半传输速率，最后趋于平衡。



所以k条经过同一节点TCP连接会平分带宽R，每条连接得到带宽R/k。

正因为如此，不论是以前的net vampire，还是现在的迅雷都采取增加并发连接数的方法来加快下载速度。

references:

- James F. Kurose, Keith W. Ross: [Computer Networking: A Top-Down Approach](https://link.zhihu.com/?target=http%3A//www.pearsonhighered.com/pearsonhigheredus/educator/product/products_detail.page%3Fisbn%3D9780132856201), 6e