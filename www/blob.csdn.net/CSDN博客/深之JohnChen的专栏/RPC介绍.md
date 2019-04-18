# RPC介绍 - 深之JohnChen的专栏 - CSDN博客

2017年07月02日 12:55:17[byxdaz](https://me.csdn.net/byxdaz)阅读数：410


服务化和微服务化渐渐成为中大型分布式系统架构的主流方式，而 RPC 在其中扮演着关键的作用。RPC的全称是 RemoteProcedure Call 是一种进程间通信方式。允许程序调用另一个地址空间（通常是共享网络的另一台机器上）的过程或函数，而不用程序员显式编码这个远程调用的细节。即程序员无论是调用本地的还是远程的，本质上编写的调用代码基本相同。它是一项广泛用于支持分布式应用程序（不同组件分布在不同计算机上的应用程序）的技术。

RPC采用客户机/服务器模式。请求程序就是一个客户机，而服务提供程序就是一个服务器。首先，客户端调用进程发送一个有进程参数的调用信息到服务进程，然后等待应答信息。在服务器端，进程保持睡眠状态直到调用信息的到达为止。当一个调用信息到达，服务器获得进程参数，计算结果，发送答复信息，然后等待下一个调用信息，最后，客户端调用进程接收答复信息，获得进程结果，然后调用执行继续进行。

![](https://pic4.zhimg.com/45366c44f775abfd0ac3b43bccc1abc3_b.jpg)

RPC调用分类，RPC调用分以下两种：

1、同步调用客户方等待调用执行完成并返回结果。

2、异步调用客户方调用后不用等待执行结果返回，但依然可以通过回调通知等方式获取返回结果。 若客户方不关心调用返回结果，则变成单向异步调用，单向调用不用返回结果。

异步和同步的区分在于是否等待服务端执行完成并返回结果。

本地调用和RPC调用的一些差异：

1. 本地调用一定会执行，而远程调用则不一定，调用消息可能因为网络原因并未发送到服务方。

2. 本地调用只会抛出接口声明的异常，而远程调用还会跑出 RPC 框架运行时的其他异常。

3. 本地调用和远程调用的性能可能差距很大，这取决于 RPC 固有消耗所占的比重。

RPC 实现

　　如webservice、thrift等，他们大部分都通过 IDL 定义，并提供工具来映射生成不同语言平台的 user-stub 和 server-stub，并通过框架库来提供 RPCRuntime 的支持。

RPC结构

![](http://static.open-open.com/lib/uploadImg/20150308/20150308144517_549.png?_=4457323)

RPC过程调用详解：RPC 服务端通过 RpcServer 去导出（export）远程接口方法，而客户端通过 RpcClient 去引入（import）远程接口方法。 客户端像调用本地方法一样去调用远程接口方法，RPC 框架提供接口的代理实现，实际的调用将委托给代理 RpcProxy 。 代理封装调用信息并将调用转交给 RpcInvoker 去实际执行。 在客户端的 RpcInvoker 通过连接器 RpcConnector 去维持与服务端的通道 RpcChannel， 并使用 RpcProtocol 执行协议编码（encode）并将编码后的请求消息通过通道发送给服务方。RPC
 服务端接收器 RpcAcceptor 接收客户端的调用请求，同样使用 RpcProtocol 执行协议解码（decode）。 解码后的调用信息传递给 RpcProcessor 去控制处理调用过程，最后再委托调用给 RpcInvoker 去实际执行并返回调用结果。

RPC各组件职责：

　　下面我们详细说明下每个组件的职责划分。

1、RpcServer 负责导出（export）远程接口。

2、RpcClient 负责导入（import）远程接口的代理实现。大部分跨语言平台 RPC 框架采用根据 IDL 定义通过 code generator 去生成 stub 代码， 这种方式下实际导入的过程就是通过代码生成器在编译期完成的。一些跨语言平台 RPC 框架如 CORBAR、WebService、ICE、AVRO、Thrift 均是此类方式。交互双方必定要在RPC框架下，用一样的IDL语言，描述逻辑，在最终业务交互中，也不会存在所谓的JAVA对象被传递到对方，由对方（可能是另外一种语言实现）来解析的情况，大家在这种场景下传递的肯定是各自能读懂的内容。

3、RpcProxy 远程接口的代理实现。

4、RpcInvoker 客户方实现：负责编码调用信息和发送调用请求到服务方并等待调用结果返回；服务方实现：负责调用服务端接口的具体实现并返回调用结果。

5、RpcProtocol 负责协议编/解码。用于协议数据的序列化与反序列化。

6、RpcConnector 负责维持客户方和服务方的连接通道和发送数据到服务方。

7、RpcAcceptor 负责接收客户方请求并返回请求结果。一般使用线程池TCP连接处理客户端的多个请求。

8、RpcProcessor 负责在服务方控制调用过程，包括管理调用线程池、超时时间等。

9、RpcChannel 数据传输通道。在TCP连接的基础上进行请求方法数据的传输，一个TCP连接创建多个数据传输通道，用于不同请求方法数据的传输。

参考资料：

深入浅出RPC

http://blog.csdn.net/mindfloating/article/details/39473807

http://blog.csdn.net/mindfloating/article/details/39474123

HDFS的RPC框架

[http://www.tuicool.com/articles/VBrQvm](http://www.tuicool.com/articles/VBrQvm)

