
# Storm的通信机制 - Magician的博客 - CSDN博客


2018年04月06日 21:35:10[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：322所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)



[目录](#目录)[前言：](#前言)
[1、Worker进程间通信原理](#1worker进程间通信原理)
[2、Worker进程间技术(Netty、ZeroMQ)](#2worker进程间技术nettyzeromq)
[3、Worker 内部通信技术(Disruptor)](#3worker-内部通信技术disruptor)
[总结：](#总结)


# 目录
## 前言：
这篇文章，博客主要介绍下Storm中Worker进程间和进程内部通信的原理和技术。整篇内容仅供了解，如有不足或错误请指出。
Worker间的通信经常需要**通过网络跨节点进行**，Storm使用ZeroMQ或Netty(0.9以后默认使用)作为进程间通信的消息框架。
Worker进程内部通信：不同worker的thread通信使用LMAX Disruptor来完成。
不同topologey之间的通信，Storm不负责，需要自己想办法实现，例如使用kafka等。
## 1、Worker进程间通信原理
worker进程间消息传递机制，消息的接收和处理的大概流程见下图
![这里写图片描述](https://img-blog.csdn.net/20180406211945804?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
1、对于worker进程来说，为了管理流入和传出的消息，每个worker进程有一个独立的接收线程<一个worker进程运行一个专用的接收线程来负责将外部发送过来的消息移动到对应的executor线程的incoming-queue中>(对配置的TCP端口supervisor.slots.ports进行监听)对应Worker接收线程，每个worker存在一个独立的发送线程（transfer-queue的每个元素实际上代表一个tuple的集合） ，它负责从worker的transfer-queue中读取消息，并通过网络发送给其他worker。
2、每个executor有自己的incoming-queue 和outgoing-queue 。
Worker接收线程将收到的消息通过task编号传递给对应的executor(一个或多个)的incoming-queues;
每个executor有单独的线程分别来处理spout/bolt的业务逻辑，业务逻辑输出的中间数据会存放在outgoing-queue中，当executor的outgoing-queue中的tuple达到一定的阀值，executor的发送线程将批量获取outgoing-queue中的tuple,并发送到transfer-queue中。
3、每个worker进程控制一个或多个executor线程，用户可在代码中进行配置。其实就是我们在代码中设置的并发度个数。
**总结：每个work都有对应的接受线程用来通过网络接收外部发送过来的消息，并根据tuple中包含的taskId匹配到对应的excutor，将该消息移动到对应的excutor线程的incoming-queue里面，excutor从incoming-queue里面拿数据进行处理后（通过bolt的excute方法来消费incoming-queue中的tuple数据），将中间结果输出到outgoing-queue中，当outgoing-queue中的数据达到一定的阈值之后，excutor的发送线程会将outgoing-queue中的数据发送到worker的transfer-queue中，而worker的发送线程再将transfer-queue中读到的消息通过网络发送给其他的worker.**
## 2、Worker进程间技术(Netty、ZeroMQ)
**2.1、Netty**
Netty是一个**NIO client-server(客户端服务器)框架**，使用Netty可以快速开发网络应用，例如服务器和客户端协议。Netty提供了一种新的方式来使开发网络应用程序，这种新的方式使得它很容易使用和有很强的扩展性。Netty的内部实现时很复杂的，但是Netty提供了简单易用的api从网络处理代码中解耦业务逻辑。Netty是完全基于NIO实现的，所以**整个Netty都是异步**的。
书籍：Netty权威指南
**2.2、ZeroMQ**
ZeroMQ是一种基于消息队列的多线程网络库，其对套接字类型、连接处理、帧、甚至路由的底层细节进行抽象，提供跨越多种传输协议的套接字。ZeroMQ是网络通信中新的一层，介于应用层和传输层之间（按照TCP/IP划分），其是一个可伸缩层，可并行运行，分散在分布式系统间。
ZeroMQ定位为：一个简单好用的传输层，像框架一样的一个socket library，他使得Socket编程更加简单、简洁和性能更高。是一个消息处理队列库，可在多个线程、内核和主机盒之间弹性伸缩。ZMQ的明确目标是“成为标准网络协议栈的一部分，之后进入Linux内核”。
## 3、Worker 内部通信技术(Disruptor)
**3.1、Disruptor的来历**
一个公司的业务与技术的关系，一般可以分为三个阶段。第一个阶段就是跟着业务跑。第二个阶段是经历了几年的时间，才达到的驱动业务阶段。第三个阶段，技术引领业务的发展乃至企业的发展。所以我们在学习Disruptor这个技术时，不得不提LMAX这个机构，因为Disruptor这门技术就是由LMAX公司开发并开源的。
LMAX是在英国注册并受到FSA监管（监管号码为509778）的外汇黄金交易所。LMAX也是欧洲第一家也是唯一一家采用多边交易设施Multilateral
Trading Facility（MTF）拥有交易所牌照和经纪商牌照的欧洲顶级金融公司
LAMX拥有最迅捷的交易平台，顶级技术支持。LMAX交易所使用“（MTF）分裂器Disruptor”技术，可以在极短时间内（一般在3百万秒之一内）处理订单，在一个线程里每秒处理6百万订单。所有订单均为撮合成交形式，无一例外。多边交易设施（MTF）曾经用来设计伦敦证券交易所（london Stock Exchange）、德国证券及衍生工具交易所（Deutsche Borse）和欧洲证券交易所（Euronext）。
2011年LMAX凭借该技术获得了金融行业技术评选大赛的最佳交易系统奖和甲骨文“公爵杯”创新编程框架奖。
3.2、Disruptor是什么
1、简单理解：Disruptor是一个Queue。Disruptor是实现了“队列”的功能，而且是一个有界队列(长度有限)。而队列的应用场景自然就是“生产者-消费者”模型。
2、    在JDK中Queue有很多实现类，包括不限于ArrayBlockingQueue、LinkBlockingQueue，这两个底层的数据结构分别是数组和链表。数组查询快，链表增删快，能够适应大多数应用场景。
3、    但是ArrayBlockingQueue、LinkBlockingQueue都是线程安全的。涉及到线程安全，就会有synchronized、lock等关键字，这就意味着CPU会打架。
4、Disruptor一种线程之间信息无锁的交换方式（使用CAS（Compare And Swap/Set）操作）。
3.2、Disruptor主要特点
1、    没有竞争=没有锁=非常快。
2、    所有访问者都记录自己的序号的实现方式，允许多个生产者与多个消费者共享相同的数据结构。
3、    在每个对象中都能跟踪序列号（ring buffer，claim Strategy，生产者和消费者），加上神奇的cache line padding，就意味着没有为伪共享和非预期的竞争。
3.3、 Disruptor 核心技术点
Disruptor可以看成一个事件监听或消息机制，在队列中一边生产者放入消息，另外一边消费者并行取出处理.
底层是单个数据结构：一个ring buffer。
每个生产者和消费者都有一个次序计算器，以显示当前缓冲工作方式。
每个生产者消费者能够操作自己的次序计数器的能够读取对方的计数器，生产者能够读取消费者的计算器确保其在没有锁的情况下是可写的。
核心组件
Ring Buffer 环形的缓冲区，负责对通过 Disruptor 进行交换的数据（事件）进行存储和更新。
Sequence 通过顺序递增的序号来编号管理通过其进行交换的数据（事件），对数据(事件)的处理过程总是沿着序号逐个递增处理。
RingBuffer底层是个数组，次序计算器是一个64bit long 整数型，平滑增长。
![这里写图片描述](https://img-blog.csdn.net/20180406212816110?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180406212816110?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Ring Buffer 运行的过程：
1、  接受数据并写入到脚标31的位置，之后会沿着序号一直写入，但是不会绕过消费者所在的脚标。
2、  Joumaler和replicator同时读到24的位置，他们可以批量读取数据到30
3、消费逻辑线程读到了14的位置，但是没法继续读下去，因为他的sequence暂停在15的位置上，需要等到他的sequence给他序号。如果sequence能正常工作，就能读取到30的数据。
总结：
以上的一些内容，只需要了解即可，目的还是为了能够更好的理解Storm内部的通信的原理，为今后的进阶之路打好基础。

[
  ](https://img-blog.csdn.net/20180406212816110?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)