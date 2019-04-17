# SPDY简介 - 囧囧有神的专栏 - CSDN博客





2014年01月07日 10:41:26[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：3123









# 　　SPDY简介

　　SPDY是Google开发的基于传输控制协议（TCP）的应用层协议 。SPDY旨在缩短网页的加载时间和提高安全性。Google最早是在Chromium中提出的SPDY协议。



目前主流浏览器Google Chrome Mozilla Firefox Opera Internet Explorer都已经支持了SPDY

主流web服务器Apache、Nginx、Netty、Jetty、node.js等都已经开始初步的支持SPDY基本已经支持SPDY


SPDY当前并不是一个标准协议，但SPDY的开发组已经开始推动SPDY成为正式标准（最新为互联网草案[4]）

本文主要了解SPDY的基本概念，以及如何编译一个含有SPDY的nginx



HTTP协议存性能上的一些问题
- 一个连接一个请求。浏览器和web server之间都是以短连接方式交互，一个连接只服务一次请求，对于一个需要加载多个资源的页面来说，将会带来很高的延迟。
- 只能由客户端发起请求。服务器不能主动的将一些必须的资源推送给客户端。
- HTTP协议只能对body进行压缩处理，不能压缩header。在一个cookie较多的站点，将对带宽造成严重的浪费。
- 冗余的头部。一些头部在同一个通道中通过请求在重复的发送。像User-Agent Host Accept* 常常是固定不变的，所以不需要重复发送
- 可选的压缩。HTTP 使用可选的压缩编码。但是内容应该总是使用压缩格式。

## SPDY的目标

SPDY为WEB定义和实现了一个应用层的协议来大大降低延迟。SPDY的高层次目标是：
- 减少50%的网页加载时间。我们的成果已经初步接近这个目标（见下面解释）
- 最小化部署复杂性。SPDY使用TCP作为相关的传输层，所以现存的网络基础设施，不需要改变
- 避免网站开发者需要对网站作出任何改变。支持SPDY唯一需要的改变在用户user agent和web server
- 聚集有兴趣探索协议来解决延迟问题的志同道合的开发者。我们希望和开源社区 行业专家一起来开发这个新的协议

一些具体的技术目标：
- 单个tcp连接支持并发的HTTP请求
- 压缩头部和去掉不必要的头部，来减少当前HTTP使用的带宽
- 定义一个容易实现，在服务器端高效率的协议。我们希望通过减少边缘情况 定义易解析的消息格式来减少HTTP的复杂性
- 让SSL协议在现存的网络基础设施下有更好的安全性和兼容性。虽然SSL确实引入了延迟，我们认为网络的长远发展依赖一个安全的网络连接。另外，使用SSL来确保整个通信不中断是必要的。


## SPDY设计和特征

在SSL层上加了一个SPDY session层，来实现并发和stream机制。

通常的HTTP GET和POST格式仍然是一样的；然而SPDY为编码和传输设计了一个新的帧格式。



### 基本特征
- 复用流 SPDY允许在一个连接上无限制的并发流。因为请求在一个通道上，TCP效率更高：更少的网络连接，更少更密集的数据包被发出
- 请求优先级 虽然无数的并行数据流解决了序列化问题，但他们引入了另外的问题
- HTTP头部压缩


### 高级特征

此外，SPDY提供了高级特征，服务器启动流。服务器启动流能用来分发内容到客户端，而不需要客户端请求它。这个选项可以由web开发人员通过如下两种方法配置：
- Server push SPDY通过X-Associated-Content头试验了服务器推送数据给客户端的选项。这个头告诉客户端服务器将在客户端请求资源之前，推送资源给它。对于初始页面下载（例如用户初次访问这个网站），这样能大大提升用户体验
- Server hint 相对于自动的推送资源到客户端，服务器使用X-Subresources头去建议客户端，来请求特殊的资源，这是在服务器事先知道客户的这些资源将被需要的情况下。但是，服务器仍然在发送内容前等待客户请求。通过慢速链接，这个选项能减少一个客户端发现它需要的资源数百毫秒的时间，并可能对非初始页面加载会更好。


## SPDY实现

下面是已经实现的：

一个能同时提供HTTP SPDY服务的高速 全内存的服务程序。我们将在不久的将来开源这些代码

一个能使用HTTP或者SPDY的chrome浏览器。

一个测试和基准设施，来确保页面是不变的。





# 　　NGINX SPDY编译


http://nginx.org/patches/attic/spdy/README.txt 

Nginx 支持 SPDY draft 2

Nginx 从1.3.15开始支持

需要OpenSSL 1.01+

目前已知的问题和限制：

 不支持server push

 不支持SPDY连接速率限制


如何编译含SPDY的nginx？

1.安装OpenSSL 1.0.1+

2.下载nginx 1.3.x 以上的版本

3.解压nginx

4.下载应用SPDY module patch

  wget http://nginx.org/patches/spdy/patch.spdy.txt

  patch -p1 < patch.spdy.txt

5.配置 

./configure --with-http_ssl_module --with-http_spdy_module

6.编译

Make


配置

server {

listen 443 ssl spdy default_server;


ssl_certificate server.crt;

ssl_certificate_key server.key;


...

}





下一步会主要学习SPDY草案的内容，以及阅读代码






http://www.chromium.org/developers/design-documents/extensions/how-the-extension-system-works/chrome-benchmarking-extension

http://dev.chromium.org/spdy/spdy-whitepaper 

http://zh.wikipedia.org/wiki/SPDY 






