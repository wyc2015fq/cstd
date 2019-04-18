# 【计算机网络】【TCP】如何讲清楚Tcp的三次握手和四次挥手？ - csdn_baotai的博客 - CSDN博客

2018年09月13日 19:51:47[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：111
个人分类：[计算机网络](https://blog.csdn.net/csdn_baotai/article/category/8051136)



### 每一次TCP连接都需要三个阶段：连接建立、数据传送和连接释放。

# 三次握手:

## 三次握手就发生在连接建立阶段。

目的：三次握手的目的是为了防止已失效的连接请求报文段突然又传送到了服务端，因而产生错误（为什么是三次握手而不是两次握手的原因） 

**为什么TCP协议建立连接要三次握手？**

> 
​   已失效的连接请求报文段的产生在这样一种情况下：client发出的第一个连接请求报文段并没有丢失，而是在某个网络结点长时间的滞留了，以致延误到连接释放以后的某个时间才到达server。本来这是一个早已失效的报文段。但server收到此失效的连接请求报文段后，就误认为是client再次发出的一个新的连接请求。于是就向client发出确认报文段，同意建立连接。假设不采用三次握手，那么只要server发出确认，新的连接就建立了。由于现在client并没有发出建立连接的请求，因此不会理睬server的确认，也不会向server发送数据。但server却以为新的运输连接已经建立，并一直等待client发来数据。这样，server的很多资源就白白浪费掉了。采用三次握手的办法可以防止上述现象发生。例如刚才那种情况，client不会向server的确认发出确认。server由于收不到确认，就知道client并没有要求建立连接。所以三次通信是理论上的最小值。所以三次握手不是TCP本身的要求, 而是为了满足”在不可靠信道上可靠地传输信息”这一需求所导致的.  　　

# 四次挥手：

## 四次挥手发生在连接释放阶段。

### 为什么TCP协议终止链接要四次？
- 当主机A确认发送完数据且知道B已经接受完了，想要关闭发送数据口（当然确认信号还是可以发），就会发FIN给主机B。
- 主机B收到A发送的FIN，表示收到了，就会发送ACK回复。
- 但这是B可能还在发送数据，没有想要关闭数据口的意思，所以FIN与ACK不是同时发送的，而是等到B数据发送完了，才会发送FIN给主机A。
- A收到B发来的FIN，知道B的数据也发送完了，回复ACK， A等待2MSL以后，没有收到B传来的任何消息，知道B已经收到自己的ACK了，A就关闭链接，B也关闭链接了。

### A为什么等待2MSL，从TIME_WAIT到CLOSE？

> 
​         在Client发送出最后的ACK回复，但该ACK可能丢失。Server如果没有收到ACK，将不断重复发送FIN片段。所以Client不能立即关闭，它必须确认Server接收到了该ACK。Client会在发送出ACK之后进入到TIME_WAIT状态。Client会设置一个计时器，等待2MSL的时间。如果在该时间内再次收到FIN，那么Client会重发ACK并再次等待2MSL。所谓的2MSL是两倍的MSL(Maximum Segment Lifetime)。MSL指一个片段在网络中最大的存活时间，2MSL就是一个发送和一个回复所需的最大时间。如果直到2MSL，Client都没有再次收到FIN，那么Client推断ACK已经被成功接收，则结束TCP连接。 

​​​​![Tcp协议状态转换](https://img-blog.csdn.net/20180912223549481?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NzZG5fYmFvdGFp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

