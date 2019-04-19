# Golang通过Thrift框架完美实现跨语言调用 - 三少GG - CSDN博客
2015年08月26日 17:40:03[三少GG](https://me.csdn.net/scut1135)阅读数：4118

　　每种语言都有自己最擅长的领域，Golang 最适合的领域就是服务器端程序。
　　做为服务器端程序，需要考虑性能同时也要考虑与各种语言之间方便的通讯。采用http协议简单，但性能不高。采用TCP通讯，则需要考虑封包、解包、粘包等等很多因素，而且想写个高效的TCP服务，也很难。
　　其实，对于此类需求，采用RPC（Remote Procedure Call Protocol）编程最靠谱。使用 RPC 编程被认为是在分布式环境中运行的客户机和服务器应用程序之间进行可靠通信的最强大、最高效的方法之一。
　　Golang内置了对RPC支持，但只能适用于go语言程序之间调用，且貌似序列化、反序列化性能不高。如果go语言能使用Thrift开发，那么就如虎添翼了。可惜，thrift虽然很早就包含了golang的代码，但一直都存在各种问题无法正确执行，以至于GitHub上有许多大牛小牛自行实现的Thrift代码，但依然各种问题……直到0.9.1版本的发布！
　　是的，最近，Apache Thrift 0.9.1正式发布了。新版的Thrift终于对Golang提供了完美的支持。经过实验，服务器端、客户端已经完美支持跨语言调用，且性能、尤其是内存占用上，编译型语言的特点展现出来，比java版的实现强了很多。
　　下面，我们采用golang实现一个Thrift的Server端和Client端程序。
http://blog.csdn.net/liuxinmingcode/article/details/45696237

#### [开源RPC（gRPC/Thrift）框架性能评测](http://www.eit.name/blog/read.php?566)
![](http://www.eit.name/blog/template/i170/images/toolbar_fontsize.gif)大 | 中 | 小![](http://www.eit.name/blog/template/i170/images/toolbar_rss.gif)
[![不指定](http://www.eit.name/blog/images/weather/blank.gif) 2015/07/31 11:06 | by [ipaddr](http://www.eit.name/blog/view.php?go=user_1) ]
*海量互联网业务系统只能依赖分布式架构来解决，而分布式开发的基石则是RPC；本文主要针对两个开源的RPC框架（gRPC、 Apache Thrift），以及配合GoLang、C++两个开发语言进行性能对比分析。C++、Thrift都是比较成熟的技术，先简单介绍一下GoLang以及gRPC；*
**GoLang**
Go语言是由Google开发的一个开源项目，目的之一为了提高开发人员的编程效率。 Go语言语法灵活、简洁、清晰、高效。它对的并发特性可以方便地用于多核处理器 和网络开发，同时灵活新颖的类型系统可以方便地编写模块化的系统。Go集成了C、Python(PHP)、ErLang等语言的优点，主要特点有：
- 面向过程的改良, 不追求极致面向对象；
- 强类型、静态编译，几乎没有部署依赖（Java需要JVM，PHP/Python需要解析执行器，与静态编译的C/C++相当）；性能优秀，与C/C++、Java同量级；
- 为分布式而生，优雅高效的并发能力，基于消息的并发和同步；
- 自动垃圾回收，不用再担心内存泄露；
- 内置各种高级语言类型，各种互联网协议和类库；
gRPC
一个高性能、通用的开源RPC框架，其由Google主要面向移动应用开发并基于HTTP/2协议标准而设计，基于ProtoBuf(Protocol Buffers)序列化协议开发，且支持众多开发语言。
gRPC基于HTTP/2标准设计，带来诸如双向流控、头部压缩、单TCP连接上的多复用请求等特性。这些特性使得其在移动设备上表现更好，更省电和节省空间占用。
（tomzhou原创，转载请注明，个人博客：[http://www.iamadmin.com/](http://www.eit.name//) ）
本次测试对象主要有三个组合：
- gRPC & GoLang
- Thrift & GoLang
- Thrift & C++
通过这三个组合，基本可以对比出gRPC/Thrift, go/c++两者在RPC下的性能；
此外，Thrift & C++还有多种服务器模式，我挑选了TSimpleServer、TNonblockingServer两种进行测试；
测试环境
CPU：Intel E5-2640 2.50GHz (8 cores)
内存：16GB
软件: CentOS 6.5，Go 1.4、Gcc 4.4.6，开启tcp reuse, tcp recycle；
**测试逻辑**
【远程加法运算】，参考IDL：
**MathService.proto**
> 
***syntax = "proto3";***
***service MathService {***
***rpc Add (AddRequest) returns (AddReply) {}***
***}***
***message AddRequest {***
***int32 A = 1;***
***int32 B = 2;***
***}***
***message AddReply {***
***int32 X = 1;***
***}***
**MathService.thrift**
> 
***service MathService {***
***i32 Add(1:i32 A, 2:i32 B)***
***}***
测试场景
- client, server都是单进程，长连接，在单次连接内发起1w（5w）次rpc调用，计算耗时；
- client, server都是单进程，短连接，共发起1w（5w）次连接，每次连接单次RPC调用，计算耗时；
- 并发4个client进程，每个进程长连接10w rpc，服务端单进程多线程（协程），计算耗时；
*由于不同语言，耗时统计存在偏差，比如boost.timer在程序里计算出来的耗时明显偏小，所以统一使用linux命令time来计算耗时；*
测试数据和分析
**一、 单进程下，长短连接，两个RPC框架和两大语言对比**
![](http://www.eit.name/blog/attachment/201507/1438311914_0.jpg)
![](http://www.eit.name/blog/attachment/201507/1438311914_1.jpg)
**小结：**
- 整体上看，长连接性能优于短连接，性能差距在两倍以上；
- 对比Go语言的两个RPC框架，Thrift性能明显优于gRPC，性能差距也在两倍以上；
- 对比Thrift框架下的的两种语言，长连接下Go 与C++的RPC性能基本在同一个量级，在短连接下，Go性能大概是C++的二倍；
- 对比Thrift&C++下的TSimpleServer与TNonblockingServer，在单进程客户端长连接的场景下，TNonblockingServer因为存在线程管理开销，性能较TSimpleServer差一些；但在短连接时，主要开销在连接建立上，线程池管理开销可忽略；
- 两套RPC框架，以及两大语言运行都非常稳定，5w次请求耗时约是1w次的5倍；
二、 多进程（线程，协程）下，两大RPC框架和两大语言对比
![](http://www.eit.name/blog/attachment/201507/1438311914_2.jpg)
![](http://www.eit.name/blog/attachment/201507/1438311914_3.jpg)
小结：
- Go语言本身的并发设计非常优秀，相关RPC框架默认支持协程和非堵塞，通过设置GOMAXPROCS可以非常容易的控制程序占用的CPU核数，编码角度无需关心并发实现；
- C++有堵塞和非堵塞的选择，同时需要自己实现线程池（Thrift自带），高并发场景下编码需要特别设计；
- Thrift框架性能比gRPC框架快两倍以上；
- 堵塞模式下的Thrift&C++组合，只能同时针对单个客户端提供服务，四个客户端依次顺序执行；高并发调用场景下，基本不太可能采用；
- 高并发场景下，使用Thrift框架，Go/C++性能相当，服务端单核处理能力可达3.2w/s。
总结：
- Go语言性能强劲，语法上灵活、简单、清晰，易于发布和部署，可大规模应用于业务、运维、测试系统；（自动GC类语言，GC势必会影响业务逻辑执行性能，具体影响待海量业务下进一步验证）
- gRPC做为Google开源出来的RPC框架，性能上明显低于Thrift； 但设计上更为规范，基于HTTP/2可以较好的网络适应及扩展，协议自带的流控等功能未能进一步测试；
