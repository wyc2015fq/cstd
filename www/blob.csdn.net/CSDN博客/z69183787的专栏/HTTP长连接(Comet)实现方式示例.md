# HTTP长连接(Comet)实现方式示例 - z69183787的专栏 - CSDN博客
2014年03月17日 09:22:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3185
昨天看了comet的介绍后，虽然大概知道了comet的原理，不过没实际用过还是不太清楚，于是今天又在网上翻了一下别的网友共享的comet实现http长连接的例子，在 [Comet Server Push 技术介绍](http://mzhou.me/2010/10/6/comet-push-http-ajax.html) 一文中分别提到了
 ”基于长轮询(long polling)“、”基于iframe“、”基于流(stream)“三种实现comet的方式，下面我将网上找到的对应的例子链接做个汇总，就当目录吧，留作以后看 
 基于长轮询(long polling)的特点 
1. 服务器端会阻塞请求直到有数据传递或超时才返回。 
2. 客户端响应处理函数会在处理完服务器返回的信息后，再次发出请求，重新建立连接。 
3. 当客户端处理接收的数据、重新建立连接时，服务器端可能有新的数据到达；这些信息会被服务器端保存直到客户端重新建立连接，客户端会一次把当前服务器端所有的信息取回。 
基于长轮询的实现方式的例子可以参见这篇文章 [Ajax轮询以及Comet模式—写在Servlet 3.0发布之前](http://www.blogjava.net/rosen/archive/2009/02/11/254309.html)
 不过个人感觉作者在写服务器端的示例代码时没有体现出 "服务器端会阻塞请求直到有数据传递或超时才返回",正确的做法应该是一个servlet接收聊天信息，一个servlet响应轮询请求，采用观察者模 式，被观察者的状态变化是查看聊天信息是否有更改，观察者会保存每个浏览器的轮询请求的response对象，当被观察者接收到新的聊天信息时，会通知观 察者，观察者则会将当前最新的聊天内容返回给浏览器。也就是说只有当接收聊天信息的servlet接收到新的聊天内容时，被观察者才会通知所有的观察者， 否则观察者将会一直保持response对象，没有返回，这样对浏览器来说就是一种阻塞。有时间自己再写一个例子验证一下... 
基于iframe 
通过在 HTML 页面里嵌入一个隐蔵帧，然后将这个隐蔵帧的 SRC 属性设为对一个长连接的请求，服务器端就能源源不断地往客户端输入数据。这种方式的难点在于如何判断连接中断并重新尝试连接。
基于iframe实现的例子可以参见这篇文章  [利用Response.Flush和iframe实现”服务器推”技术](http://www.cnblogs.com/zhaozhan/archive/2010/09/07/1821021.html)
基于流(stream) 
后台服务器需要支持comet的web服务器，JAVA的话可以采用Jetty+tomcat 6.0以上的组合； 浏览器端依赖于Streaming AJAX 技术，目前Mozilla Firefox 提供了对这种技术的支持， 即当xmlhttprequest的返回值readystate 为 3 时（数据仍在传输中），客户端可以读取数据，从而无须关闭连接，就能读取处理服务器端返回的信息。IE 在 readystate 为 3 时，不能读取服务器返回的数据，目前 IE 不支持基于 Streaming AJAX。
基于流(stream)实现的例子可以参见这篇文章  [浅析Comet技术在Java Web实时系统开发中的应用](http://software.intel.com/zh-cn/articles/comet-java-realtime-system-essay/)
