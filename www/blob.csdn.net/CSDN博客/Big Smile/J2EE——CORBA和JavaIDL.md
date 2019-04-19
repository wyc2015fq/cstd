# J2EE——CORBA和JavaIDL - Big Smile - CSDN博客
2017年01月16日 20:45:12[王啸tr1912](https://me.csdn.net/tr1912)阅读数：384
        说到了IDL就不得不说CORBA了，因为他们是不可分割的一部分，由于JavaIDL是一个ORB，是一个用来访问CORBA的对象，或者说是一个CORBA对象的一个瞬间状态，所以在讨论IDL之前我们不得不讨论一下CORBA了。
# 一、CORBA
        CORBA（Common Object Request Broker Architecture,公共对象请求代理体系结构，通用对象请求代理体系结构）是由OMG组织制订的一种标准的面向对象应用程 序体系规范。或者说 CORBA体系结构是对象管理组织（OMG）为解决分布式处理环境(DCE)中，硬件和软件系统的互连而提出的一种解决方案；OMG组织是一个国际性的非盈利组织，其职责是为应用开发提供一个公共框架，制订工业指南和对象管理规范，加快对象技术的发展。
        在CORBA体系规范中定义了多种类型的服务（Service），如命名（Naming）、生存期（LifeCycle）、事件（Event）、事务（Transaction）、对象持久化（Persistent Objects）、查询（Query）、特征（Property）、时间（Time）等服务功能。　在CORBA规范中，没有明确说明不同厂商的中间件产品要实现所有的服务功能，并且允许厂商开发自己的服务类型。因此, 不同厂商的ORB产品对CORBA服务的支持能力不同，使我们在针对待开发系统的功能进行中间件产品选择时，有更多的选择余地。
        他的体系（盗图）：
![](https://img-blog.csdn.net/20170116203617889?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 二、JavaIDL
        Java IDL（Interface Definition Language，接口定义语言）为 JavaTM 平台添加了 CORBA（Common Object Request Broker Architecture，公用对象请求代理体系结构）功能，从而可提供基于标准的互操作性和连接性。
        Java IDL 使分布式、支持 Web 的 Java 应用程序可利用 Object Management Group 定义的行业标准 OMG IDL（Object Management Group Interface Definition Language，对象管理组接口定义语言）及 IIOP（Internet Inter-ORB Protocol，Internet对象请求代理间协议）来透明地调用远程网络服务。
        运行时组件包括一个全兼容的 Java ORB，用于通过 IIOP 通讯进行分布式计算。要使用 Java IDL，用户必须具有 idltojava 编译器。
         官方的说法很多，不在赘述，这里的IDL主要作为接口，对于上面提到的CORBA进行访问，具体的好处就是作为接口可以让不同对象之间进行通信和数值的传递，大大简化我们的编程步骤。
