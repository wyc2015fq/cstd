# Mina NIO与BIO了解及Telnet简单测试-Getting Started - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月01日 16:10:04[boonya](https://me.csdn.net/boonya)阅读数：3090
所属专栏：[Apache Mina学习实践](https://blog.csdn.net/column/details/i-love-mina.html)









原文地址：[Mina user guide getting started](http://mina.apache.org/mina-project/userguide/ch1-getting-started/ch1-getting-started.html)


### 1.1.1. **NIO回顾**

#### 1.1.1.1. **NIO回顾**

  NIO API是java 1.4引入的，已经被大多数应用系统所采用。NIO API包含了非阻塞non-blocking应用操作。

**java.nio.*包中包含了以下关键的结构：**
- l Buffers - 数据容器
- l Chartsets -  bytes 和Unicode的翻译容器
- l Channels - 代表连接与实体的IO操作能力
- l Selectors - 提供可选择的多个的非阻塞(non-blocking )IO选择器
- l Regexps - 提供常规的正则表达式工具

在Mina框架中我们最感兴趣的是channels_,_selectors_ 和Buffers，尽管我们想把这些元素对用户隐藏起来。本用户指南将因此致力于构建一切之上的内部组件。

#### 1.1.1.2. **NIO对比BIO**

  理解NIO与BIO之间的区别是很重要的。BIO就是发起一个简单的Socket连接的阻塞模式：当读取或写入数据时，无论在socket调用期间执行了什么操作，被调用的方法处理会被阻塞，直到调用处理完成为止。

        在某些情况下，能够调用处理操作很重要，否则就必须等待操作完成。如果调用完成才能进行某些处理，否则就会持续等待，这就意味着时间。

 这里就是NIO需要提供更好的处理方式来解决大量的Socket连接：你不必为每一个连接创建一个线程，而只需要创建很少量的线程来做相同的事情。

        如果你想更多了解哪些包含了NIO的信息，网上有很多文章和少量书籍可以解决这个问题。

### 1.1.2. **为什么是Mina**

#### 1.1.2.1. **为什么是Mina**

         编写一些网络应用，通常被视为是一种负担和低水平的发展。它不是一个被开发人员频繁学习和了解的领域，也因为在学校学习的时间太久而被忘记，或者是因为网络层的复杂和隐藏性而被更高级别的层级所掩盖，所以你从来都没有深入去研究它。

随着异步IO的引入，其他层级的复杂性也显现出来了，如：时间。

  BIO与NIO之间大的区别是在BIO上。你发送一个请求，然后等待直到获取到响应为止。在服务端，它意味着线程会关联上任何连入进来的连接，所以你不会去处理复杂的多路连接。对于NIO而言，另一方面，你不得不处理异步特性的非阻塞系统，这就意味着当事件触发时你的应用程序就会被调用。在NIO中，你不会调用和等待一个返回结果，而是通过命令来通知结果是否完成。

#### 1.1.2.2. **需要一个框架**

         思虑这些区别，事实上大多数的应用都期待在网络层是阻塞模式，最好的方案就是写一个框架来模拟（mimic）阻塞模式，这就是Mina。

         但是Mina做得更多，它提供了基本的IO通信版本如，TCP、UDP、或无论什么底层原理。如果我们只考虑TCP和UDP，TCP是一个连接协议，当另一个是非连接的（UDP）。Mina做了不同的实现，并且能够使你只需要关注两个方面：及请求编码和应用协议编、解码。

  Mina 不仅处理TCP和UDP，它也为串口通信提供上层，如**RSC232，VmpPipe，APR。**

         最后，至少Mina是一个客户端和服务端的网络通信框架。编写一个服务器关键是使它成为一个可伸缩的系统，适应服务端的需求，在性能和内存使用方面，这就是Mina的好处，很容易扩展服务器端。

#### 1.1.2.3. **什么时候使用Mina**

这是一个有趣的问题。Mina在任何情况下都不预期成为最合适的选择。这里有几个使用Mina的要素考虑，下面列举他们：
- 易用性当你没有特殊的性能要求,Mina可能是一个不错的选择,因为它允许你轻易扩展服务器或客户端,而不需要处理各种参数和用例编写相同的应用程序时处理的BIO或NIO。你可以写几十行代码定义你的服务器,并且可能实现一些陷阱的下降。
- 大量的连接用户采用BIO连接，NIO明显更快。不同之处在于30%的BIO。这是真的长达几千连接的用户,但在一定程度上,BIO方法只是停止扩展:你不能够处理数以百万计的连接用户使用一个线程每用户!但是NIO可以。现在,另一个方面是,Mina的时间代码可能是不重要的一部分,而不管你的应用程序将消费。在某种程度上,它可能不是多次可贵的花更多的精力写自己更快的网络层的获得，几乎不会引起注意。
- 有数据表明Mina被世界各地的数以千万计的应用程序使用。有一些基于Apache Mina项目,并且他们工作得很好。这是某种形式的保证您什么不需要花几个小时在一些神秘的错误在您自己的网络层的实现。
- 存在支持协议Mina，附带各种现有的协议实现的如:HTTP、XML、TCP、LDAP、DHCP,NPT,DNS,XMPP,SSH、FTP……在某种程度上,不仅可以看到Mina NIO框架,而是作为一个网络层协议实现。Mina的特性之一,在不久的将来是为你提供一组可以使用的现有协议。

### 1.1.3. **特性**

Mina是一个全功能的网络应用框架：
- **· 统一的API 用于各种各样的传输类型:**
- o TCP/IP & UDP/IP 的 Java NIO
- o Serial communication串口通信 (RS232) via RXTX
- o In-VM pipe communication
- o You can implement your own!
- **· Filter过滤器接口用于用户扩展过滤; 类似于Servlet 过滤器**
- · 低级和高级API:
- o 低级特性: 使用ByteBuffers
- o 高级特性: 使用统一的消息对象和编码器
- **· 高级传统线程模块:**
- o Single thread单线程
- o One thread pool线程池
- o More than one thread pools (i.e. SEDA)多线程池
- **· 开箱即用的SSL 路 TLS 路 StartTLS support using Java 5 SSLEngine**
- **· 过载保护与流量调节**
- **· 单元测试使用模拟对象**
- **· 基于流的 I/O支持流的IoHandler**
- **· 与知名容器的集成such as PicoContainer and Spring**
- **· 从Netty平滑迁移, Netty的始祖就是Apache MINA.**

### 1.1.4. **第一步**

   我们将展示你是怎样容易使用Mina,运行一个非常简单的例子并提供了Mina包。当您想要在您的应用程序使用Mina，你必须做的第一件事情是设置你的环境。我们将描述您需要安装什么以及如何运行Mina的程序。没有花哨的,只是第一次Mina的感觉…

#### 1.1.4.1. **下载**

     首先,你必须从下载部分下载最新的Mina发布版本,[here](http://mina.apache.org/mina-project/downloads.html)。获取最新的版本,除非你有很好的理由不这样做……

    一般来说,如果你要使用Maven构建您的项目,你甚至不需要下载任何东西,只要你添加依赖仓库包含Mina库:你只要告诉Maven pom要使用所需要的Mina jar。

#### 1.1.4.2. **包含文件**

下载完成后,提取压缩文件的内容。 tar.gz或zip文件到本地硬盘。下载的压缩文件有以下内容：

**在Unix系统输入，如：**
`<span style="font-size:12px;">$ tar xzpf apache-mina-2.0.7-tar.gz</span>`
**在apache-mina-2.0.7目录，你会获得如下所示目录结构：**

 +- dist

 +- docs

 +- lib

 +- src

 +- LICENSE.txt

 +- LICENSE.jzlib.txt

 +- LICENSE.ognl.txt

 +- LICENSE.slf4j.txt

 +- LICENSE.springframework.txt

 +- NOTICE.txt

#### 1.1.4.3. **内容详情**
- · *dist* - 包含**MINA** jars及其代码
- · *docs* - 包含API文档和代码外部参照
- · *lib* - 包含所有使用Mina需要的 jar包

#### 1.1.4.4. **运行第一个Mina程序**

使用下载下来的release包，并运行它的示例程序。

你需要把如下Jar包加入到classpath:
- · mina-core-2.0.7.jar
- · mina-example-2.0.7.jar
- · slf4j-api-1.6.6.jar
- · slf4j-log4j12-1.6.6.jar
- · log4j-1.2.17.jar

**日志提醒：**
- · **Log4J 1.2 users: slf4j-api.jar, slf4j-log4j12.jar, and Log4J 1.2.x**
- · **Log4J 1.3 users: slf4j-api.jar, slf4j-log4j13.jar, and Log4J 1.3.x**
- · **java.util.logging users: slf4j-api.jar and slf4j-jdk14.jar**

**重要:** 请确认使用正确的 *slf4j-*.jar* 与你使用匹配的日志框架.例如,*slf4j-log4j12.jar* 和*log4j-1.3.x.jar* 不能同时使用, 会产生冲突. 如果你不需要日志框架你可以使用*slf4j-nop.jar* 对于没有日志或*slf4j-simple.jar* 是最基本的日志框架.

**在命令提示符下,发出以下命令:**
`<span style="font-size:12px;">$ java org.apache.mina.example.gettingstarted.timeserver.MinaTimeServer</span>`
这将启动服务器。现在telnet和看到执行的程序，发出以下命令telnet：

telnet** 127**.**0**.**0**.**1 9123**

**这里的测试可以参考我的另一篇文章：**

# [Mina快速编码测试验证示例](http://blog.csdn.net/boonya/article/details/51554753)



好的,我们已经运行我们的第一个Mina程序。请尝试其他示例程序附带米娜作为例子.

### 1.1.5. **总结**

在这一章,我们看了Mina建立应用程序架构,客户端以及服务器。我们还实现了样例的TCP server/client,UDP server和client。在接下来的章节我们将讨论Mina核心结构和高级主题。







