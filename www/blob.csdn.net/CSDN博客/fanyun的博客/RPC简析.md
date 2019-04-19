# RPC简析 - fanyun的博客 - CSDN博客
2017年03月31日 12:33:11[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：278
RPC简介：
RPC（Remote Procedure Call Protocol）——[远程过程调用](http://baike.baidu.com/view/431455.htm)协议，它是一种通过[网络](http://baike.baidu.com/view/3487.htm)从远程计算机程序上请求服务，而不需要了解底层网络技术的协议。[RPC协议](http://baike.baidu.com/view/1787799.htm)假定某些[传输协议](http://baike.baidu.com/view/441895.htm)的存在，如TCP或UDP，为通信程序之间携带信息数据。在OSI[网络通信](http://baike.baidu.com/view/538641.htm)模型中，RPC跨越了[传输层](http://baike.baidu.com/view/239605.htm)和[应用层](http://baike.baidu.com/view/239619.htm)。RPC使得开发包括网络[分布式](http://baike.baidu.com/view/402382.htm)多程序在内的应用程序更加容易。
RPC采用客户机/服务器模式。请求程序就是一个客户机，而服务提供程序就是一个服务器。首先，客户机调用进程发送一个有进程参数的调用信息到服务进程，然后等待应答信息。在服务器端，进程保持睡眠状态直到调用信息到达为止。当一个调用信息到达，服务器获得进程参数，计算结果，发送答复[信息](http://baike.baidu.com/view/1527.htm)，然后等待下一个调用信息，最后，客户端调用进程接收答复信息，获得进程结果，然后调用执行继续进行。
有多种 RPC模式和执行。最初由Sun公司提出。IETF ONC宪章重新修订了Sun版本，使得ONC
 RPC协议成为IETF标准协议。现在使用最普遍的模式和执行是开放式软件基础的分布式计算[环境](http://baike.baidu.com/view/13655.htm)（DCE）。
二.RPC工作原理
![](https://img-blog.csdn.net/20170306091419726?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
运行时,一次客户机对服务器的RPC调用,其内部操作大致有如下十步：
1.调用客户端句柄；执行传送参数
2.调用本地系统[内核](http://baike.baidu.com/view/1366.htm)发送网络消息
3.[消息传送](http://baike.baidu.com/view/272926.htm)到远程[主机](http://baike.baidu.com/view/23880.htm)
4.服务器句柄得到消息并取得参数
5.执行远程过程
[](http://baike.baidu.com/pic/%E8%BF%9C%E7%A8%8B%E8%BF%87%E7%A8%8B%E8%B0%83%E7%94%A8%E5%8D%8F%E8%AE%AE/6893245/0/18d8bc3eb13533fadd93e964a9d3fd1f41345b56?fr=lemma&ct=single)
6.执行的过程将结果返回服务器句柄
7.服务器句柄返回结果，调用远程系统[内核](http://baike.baidu.com/view/1366.htm)
8.消息传回[本地主机](http://baike.baidu.com/view/544310.htm)
9.客户句柄由内核接收消息
10.客户接收句柄返回的数据
RPC OVER HTTP
Microsoft RPC-over-HTTP 部署(RPC over HTTP)允许RPC客户端安全和有效地通过Internet连接到RPC服务器程序并执行[远程过程调用](http://baike.baidu.com/view/431455.htm)。这是在一个名称为RPC-over-HTTP代理，或简称为RPC代理的[中间件](http://baike.baidu.com/view/23710.htm)的帮助下完成的。
RPC 代理运行在IIS计算机上。它接受来自Internet的RPC请求，在这些请求上执行认证，检验和访问检查，如果请求通过所有的测试，RPC代理将请求转发给执行真正处理的RPC [服务器](http://baike.baidu.com/view/899.htm)。通过RPC
 over HTTP，RPC客户端不和服务器直接通信，它们使用RPC代理作为中间件。
