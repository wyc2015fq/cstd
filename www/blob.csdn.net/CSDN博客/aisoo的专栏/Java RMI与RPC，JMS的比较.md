# Java RMI与RPC，JMS的比较 - aisoo的专栏 - CSDN博客
2012年12月03日 16:48:55[aisoo](https://me.csdn.net/aisoo)阅读数：723
远程对象方法调用并不是新概念，远程过程调用 (RPC) 已经使用很多年了。远程过程调用被设计为在应用程序间通信的平台中立的方式，它不理会操作系统之间以及语言之间的差异。即 RPC 支持多种语言，而 RMI 只支持 Java 写的应用程序。 [1]
另外 RMI 调用远程对象方法，允许方法返回 Java 对象以及基本数据类型。而 RPC 不支持对象的概念，传送到 RPC 服务的消息由外部数据表示 (External Data Representation, XDR) 语言表示，这种语言抽象了字节序类和数据类型结构之间的差异。只有由 XDR 定义的数据类型才能被传递， RPC 不允许传递对象。可以说 RMI 是面向对象方式的 Java RPC 。
Java 消息服务 ( Java Messaging Service, JMS ) 是一种允许应用程序创建、发送、接受和读取消息的Java API 。 JMS 与 RMI 的区别在于，采用 JMS 服务，对象是在物理上被异步从网络的某个 JVM 上直接移动到另一个 JVM 上。而 RMI 对象是绑定在本地 JVM 中，只有函数参数和返回值是通过网络传送的。
CORBA 是 90 年代初有 OMG 组织提出的一个分布式互操作标准，属于语言中立的。而 RMI 直接把分布式对象模型嵌入到 Java 语言的内部，使得 Java程序员可以自然的编写分布式程序，不必离开 Java 环境，或者涉及 CORBA IDL 以及 Java 到 CORBA 的类型转换。然而 RMI 不遵守 CORBA 标准，基本上是Java-to-Java 技术，难以实现与其他语言编写的对象之间的互操作。 [1]
RMI 和 CORBA 常被视为相互竞争的技术，因为两者都提供对远程分布式对象的透明访问。但这两种技术实际上是相互补充的，一者的长处正好可以弥补另一者的短处。 RMI 和 CORBA 的结合产生了 RMI-IIOP， RMI-IIOP 是企业服务器端 Java 开发的基础。
1997 年， IBM 和 Sun Microsystems 启动了一项旨在促进 Java 作为企业开发技术的发展的合作计划。两家公司特别着力于如何将 Java 用作服务器端语言，生成可以结合进现有体系结构的企业级代码。所需要的就是一种远程传输技术，它兼有 Java 的 RMI （ Remote Method Invocation ，远程方法调用）较少的资源占用量和更成熟的 CORBA （ Common Object
 Request Broker Architecture ，公共对象请求代理体系结构）技术的健壮性。出于这一需要， RMI-IIOP问世了，它帮助将 Java 语言推向了目前服务器端企业开发的主流语言的领先地位 。
DCOM 是从 COM 改造过来的， COM 这一技术部分是作为规范，定义了对象实现的二进制标准，用于单机上应用之间的通信，对象实现与使用的语言无关。 DCOM 是 COM 的分布式扩展，在 DCE RPC 之上构造对象的远程过程调用层支持对远程对象的访问。一个 DCOM 对象是支持一个或多个接口的组件。DCOM 对象不支持对象 ID ，因此，客户程序不能与某个特定的对象发生联系。
