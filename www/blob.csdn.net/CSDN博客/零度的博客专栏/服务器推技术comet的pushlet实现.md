# 服务器推技术comet的pushlet实现 - 零度的博客专栏 - CSDN博客
2016年07月18日 09:12:17[零度anngle](https://me.csdn.net/zmx729618)阅读数：361
      目前在做的平台是有关管控的产品，其实很多监控系统和即时通信系统都需要将后台发生的变化实时传送到客户端而无须客户端不停的刷新、发送请求。
       其实这就要涉及到服务器推技术，这篇博客也介绍一个实现服务器推技术的框架pushlet。在介绍之前先来了解一下传统的web应用模型和基于ajax的模型的对比。
       在传统的web应用中浏览器的主要工作就是发送请求、解析服务器返回的信息以不同的风格显示的作用。
![](https://img-blog.csdn.net/20140125204556875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGZzZjgwMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       随着技术的发展，ajax出现了，它的出现是通过浏览器发送异步请求，提高了用户操作的响应性。
![](https://img-blog.csdn.net/20140125204602812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGZzZjgwMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       但是上面的两种情况都是让用户在过时的信息下进行操作，所以实时的效果还是需要其他的技术来做实现。
       这时候基于HTTP长连接、无须在浏览器安装插件的服务器推技术就得到了发展，这种技术成为Comet。
       Comet技术的实现方式有两种：基于 AJAX 的长轮询（long-polling）方式(long-polling)和流方式(streaming)
       一、长轮询:
        HTTP的连接保持,服务器端会阻塞请求,直到服务器端有一个事件触发或者到达超时。客户端在收到响应后再次发出请求,重新建立连接。通过这种方式,服务器可以在数据可用的任何时候将数据“推”到客户端。因为这种方案基于AJAX,请求异步发出,无须安装插件,IE、Mozilla FireFox都支持。
       使用 AJAX 实现“服务器推”与传统的 AJAX 应用不同之处在于：
       1. 服务器端会阻塞请求直到有数据传递或超时才返回。
       2.客户端 JavaScript 响应处理函数会在处理完服务器返回的信息后，再次发出请求，重新建立连接。
       3. 当客户端处理接收的数据、重新建立连接时，服务器端可能有新的数据到达；这些信息会被服务器端保存直到客户端重新建立连接，客户端会一次把当前服务器端所有的信息取回。
       二、流方式:
       在流方式中,服务器推数据返回客户端,但不关闭连接,连接始终保持,直到超时,超时后通知客户端重新建立连接,并关闭原来的连接。
**Pushlet就是comet思路的一种技术实现。并且它支持了两种comet的实现方式。在pushlet里面，它提供了基于  AJAX 的 JavaScript 库文件用于实现长轮询方式的“服务器推”；还提供了基于 iframe 的 JavaScript 库文件用于实现流方式的“服务器推”。这两种方式正好是comet的两种实现方式。**
       从设计模式的角度来看，pushlet使用的是观察者模式。客户端发送请求，订阅感兴趣的事件；服务器端为每个客户端分配一个会话 ID 作为标记，事件源会把新产生的事件以多播的方式发送到订阅者的事件队列里。
       最后关于pushlet的一个demo，下载地址：[http://download.csdn.net/detail/lfsf802/6883825](http://download.csdn.net/detail/lfsf802/6883825)，希望
 能够帮助理comet和pushlet。
