# QUIC浅析

2018年06月14日 22:55:15 [啊浪的博客](https://me.csdn.net/b2222505) 阅读数：7248



 版权声明：本文为博主原创文章，未经博主允许不得转载。如文章对您有帮助，欢迎扫描上面或者头像上的二维码支持，谢谢～ https://blog.csdn.net/b2222505/article/details/79391430

# 简介

QUIC（Quick UDP Internet Connections)基于UDP的传输层协议，提供像TCP一样的可靠性。在提高web应用性能上，可以选择在应用层使用HTTP2.0实现多路传输，在物理层使用CDN解决网络拥塞和最后一公里问题。在传输层，目前主要使用TCP，但由于TCP本身的问题（一个充满补丁的丑陋的协议），成为了限制web应用性能的一个瓶颈。

# 优势

## 避免前序包阻塞（HOL阻塞）

多个数据在TCP连接上传输时，若一个数据包出现问题，TCP需要等待该包重传后，才能继续传输其它数据包。但在QUIC中，因为其基于UDP协议，UDP数据包在出问题需要重传时，并不会对其他数据包传输产生影响。

![这里写图片描述](https://img-blog.csdn.net/20180227174649334?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYjIyMjI1MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 零RTT建立连接

目前TCP与SSL/TLS(1.0,1.1,1.2)每次建连需要TCP三次握手+安全握手，需要4~5个RRT

### 第一次连接

![这里写图片描述](https://img-blog.csdn.net/20180227174710125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYjIyMjI1MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

客户端之前没有连接个此服务器，那么他会发送一个Hello Packet。服务器接到之后，会回复一个数据包。里面包含了安全证书和对此客户端唯一的SYN cookie。客户端接到包之后，首先要做的就是解码，保存好SYN cookie。SYN cookie 类似于令牌，能够验证客户端身份。它的生存周期较短，防止被盗用。这样建立连接只需要1个RTT。

当客户端接到服务器发来的第一个数据包，没有正确解码，那么它会再次发送一个包要求服务器从新发送它的安全证书，并将SYN cookie附加到这个请求包中，以便让服务器验证请求的正确性和有效性。此时，建立连接需要2个RTT。

### 重复连接

![这里写图片描述](https://img-blog.csdn.net/20180227174739200?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYjIyMjI1MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

因为客户端之前已经成功和服务器通信。自然保留了一份服务器的安全证书。当再次想要连接服务器的时候，客户端假设这个安全证书没有过期，还是有效的。加密一个Hello Packet并发送之后。接着不用等回复就可以直接加密其他的数据包并发送。Hello Packet 里面包括一些协商信息和对自己掌握着Client IP的证明等。因为不用等待确认，为了预防丢包等问题，Hello Packet可能会隔一段时间被重传多次，保证减少丢包造成的延迟。比如，先发一个Hello包，之后发送数据包，再发送一个Hello包。

服务器接到Hello包之后，用自己现有的秘钥解码，如果解码不成功，将把客户端的连接当做第一次连接，重发安全证书等信息。同上介绍的一样。此时，通常会有2个RTT，极端情况下是3个RTT。

服务器成功解码之后，验证了客户端的安全性之后，就可以继续处理接下来收到的数据包。此时延时是0个RTT。

## 优雅的丢包处理

### FEC前向纠错

QUIC协议的每个数据包除了本身的数据以外，会带有其他数据包的部分数据，在少量丢包的情况下，可以使用其他数据包的冗余数据完成数据组装而无需重传，从而提高数据的传输速度。具体实现类似于RAID5，将N个包的校验和（异或）建立一个单独的数据包发送，这样如果在这N个包中丢了一个包可以直接恢复出来。除此之外还可以用来校验包的正确性

### 关键包发送多次

![这里写图片描述](https://img-blog.csdn.net/20180227174834470?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYjIyMjI1MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 快速重启会话（支持手机网络切换）

普通基于tcp的连接，是基于两端的ip和端口和协议来建立的。在网络切换场景，例如手机端切换了无线网，使用4G网络，会改变本身的ip，这就导致tcp连接必须重新创建。而QUIC协议使用特有的UUID来标记每一次连接，在网络环境发生变化的时候，只要UUID不变，就能不需要握手，继续传输数据。

## 安全

1. 前向安全。即使被人抓包存储起来，在未来某个时间点秘钥被破解后仍然不能解密。
2. 内置的加密模块，支持SNI，因此支持一个IP部署多个证书，默认打开，相比TLS更高效的向前加密。
3. <https://www.cnblogs.com/mod109/p/7372577.html>

# 适用场景

- 长距离传输
- 手机网络
- 请求的页面资源数较多，并发的连接数多
- 要求加密传输