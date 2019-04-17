# 【面经笔记】TCP流量控制、阻塞控制 - xiaxzhou的博客 - CSDN博客





2017年07月30日 17:49:09[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：505








### 流量控制：窗口控制

滑动窗口协议的基本原理就是在任意时刻，发送方都维持了一个连续的允许发送的帧的序号，称为发送窗口；同时，接收方也维持了一个连续的允许接收的帧的序号，称为接收窗口。

![这里写图片描述](https://img-blog.csdn.net/20160510210252473)

发送方的窗口大小由接受方确定，目的在于控制发送速度。发送端根据接收端的实际接收能力控制发送的数据量，接收端主机向发送端主机通知自己可以接收的数据的大小，发送端会发送不超过这个限度的数据。

发送端会不时发送窗口探测，防止窗口更新通知丢失造成无法继续通信。

### 滑动窗口的大小具体是怎么变化？

[http://blog.csdn.net/farmwang/article/details/73521663](http://blog.csdn.net/farmwang/article/details/73521663)
- 滑动窗口大小：

**TCP的滑动窗口大小实际上就是socket的接收缓冲区大小的字节数**
- 滑动窗口移动规则：

窗口合拢：这种现象发生在数据被发送和确认时。

窗口张开：这种现象发生在另一端的接收进程读取已经确认的数据并释放了TCP的接收缓存时。

### 阻塞控制：慢启动

> 
[http://blog.csdn.net/jtracydy/article/details/52366461](http://blog.csdn.net/jtracydy/article/details/52366461)


慢启动通过观察到新分组进入网络的速率应该与另一端返回确认的速率相同而进行工作。

慢启动为发送方增加了**另一个窗口**：拥塞窗口

TCP连接建立时，拥塞窗口初始化为1个报文段，每接收到一个ACK，拥塞窗口就指数增加。

**发送方取拥塞窗口与通知窗口中的最小值作为发送的上限。**

> 
拥塞窗口是**发送方**的流量控制，而通知窗口则是**接收方**使用的流量控制。

- 拥塞避免 

为了防止cwnd增加过快而导致网络拥塞，所以需要设置一个慢启动阈值ssthresh，它的用法：

> - 
当cwnd > ssthresh,使用慢启动算法，

- 
当cwnd < ssthresh,使用拥塞控制算法，停用慢启动算法。

- 
当cwnd = ssthresh，这两个算法都可以。



TCP通信刚开始时，没有慢启动阈值，在超时重发时，才会设置为当前拥塞窗口大小的一半。

拥塞控制算法：让cwnd缓慢的增加而不是加倍的增长，每经历过一次往返时间就使cwnd增加1，而不是加倍，这样使cwnd缓慢的增长，比慢启动要慢的多。
- 快重传：

当发生三次重复确认，即使没有超时，也会重传，叫做快速重传。重传完毕，阀值减半，就像发生超时一样。

![这里写图片描述](https://img-blog.csdn.net/20160909153345254?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- 快恢复：

当发送方连续接收到三个确认时，就执行乘法减小算法，把cwnd值设置为ssthresh减半后的数值，然后开始执行拥塞避免算法（“加法增大”），使拥塞窗口缓慢地线性增大。

![这里写图片描述](https://img-blog.csdn.net/20160909153222894?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

为啥是三个ACK确认：假如只是一些报文段的重新排序，则重新排序的报文段被处理并产生一个新的ACK之前，可能产生1~2个重复的ACK。如果连续3个ACK，则非常有可能是报文丢失了。

### 画出慢启动的曲线图

乘法减小：无论在慢启动阶段还是在拥塞控制阶段，只要网络出现超时，就是将cwnd置为1，ssthresh置为cwnd的一半，然后开始执行慢启动算法（`cwnd<ssthresh`）。

加法增大：当网络频发出现超时情况时，ssthresh就下降的很快，为了减少注入到网络中的分组数，而加法增大是指执行拥塞避免算法后，是拥塞窗口缓慢的增大，以防止网络过早出现拥塞。

![这里写图片描述](https://img-blog.csdn.net/20160909153411895?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**慢启动-超时-慢启动-拥塞控制-快重传-快恢复-超时-慢启动-拥塞控制**：

![这里写图片描述](https://img-blog.csdn.net/20170804221945238?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlheHpob3U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 限流：

有一些爬虫IP不断的访问美团网站，现在美团设定一个IP在5分钟之内访问美团网站超过100次，就判定为爬虫IP，怎么设计这个程序？如果100改成10000，怎么设计？
- 令牌桶

在 Wikipedia 上，令牌桶算法是这么描述的：

每秒会有 r 个令牌放入桶中，或者说，每过 1/r 秒桶中增加一个令牌 

桶中最多存放 b 个令牌，如果桶满了，新放入的令牌会被丢弃 

当一个 n 字节的数据包到达时，消耗 n 个令牌，然后发送该数据包 

如果桶中可用令牌小于 n，则该数据包将被缓存或丢弃- 滑动窗口

控制窗口右边沿的移动速度，（实质其实和令牌桶一样）。




