# Java RPC - Simple 专栏 - CSDN博客
2012年02月03日 14:50:35[Simple_Zz](https://me.csdn.net/love284969214)阅读数：1571
所属专栏：[Java 浅谈](https://blog.csdn.net/column/details/27397.html)
### 什么是 RPC
RPC（Remote Procedure Call Protocol）远程过程调用协议。
通俗的描述是：客户端在不知道调用细节的情况下，调用存在于远程计算机上的某个过程或函数，就像调用本地应用程序中的一样。
正式的描述是：一种通过网络从远程计算机程序上请求服务，而不需要了解
底层网络技术的协议。
### RPC  主要 特质
RPC 是协议：协议意味着规范。目前典型的 RPC 实现包括：Dubbo、Thrift、Hetty 等。但这些实现往往都会附加其他重要功能，例如 Dubbo 还包括了服务管理、访问权限管理等功能。
网络协议和网络 IO 模型对其透明：既然 RPC 的客户端认为自己是在调用本地对象。那么传输层使用的是 TCP/UDP 还是 HTTP 协议，又或者是一些其他的网络协议它就不需要关心了。既然网络协议对其透明，那么调用过程中，使用的是哪一种网络 IO 模型调用者也不需要关心。
信息格式对其透明：远程调用过程中，需要传递一些参数，并且会返回一个调用结果。至于这些参数会以某种信息格式传递给网络上的另外一台计算机，这个信息格式是怎样构成的，调用方是不需要关心的。
跨语言能力：对于调用方来说，不知道也无需知道远程的程序使用的是什么语言运行的，无论服务器方使用的是什么语言，本次调用都应该成功，并且返回值也应该按照调用方程序语言所能理解的形式进行描述。
### RPC  原理
实现 RPC 的程序包括 5 个部分：User、User-stub、RPCRuntime、Server-stub、Server。
![](https://img-blog.csdn.net/20180912120147230?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xvdmUyODQ5NjkyMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)RPC 原理
user 就是发起 RPC 调用的 client ，当 user 想发起一个远程调用时，它实际是通过本地调用 user-stub。 user-stub 负责将调用的接口、方法和参数通过约定的协议规范进行编码并通过本地的 RPCRuntime 实例传输到远端的实例。远端 RPCRuntim 实例收到请求后交给 server-stub 进行解码后发起本地端调用，调用结果再返回给 user 端。
stub：为屏蔽客户调用远程主机上的对象，必须提供某种方式来模拟本地对象，这种本地对象称为存根(stub)，存根负责接收本地方法调用，并将它们委派给各自的具体实现对象。
